// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Character/Phase.h"
#include "SevarogAnimInstance.h"
#include "SevarogAIController.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Kismet/GamePlayStatics.h"
#include "Math/Vector.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Attribute/AttributeComponent.h"

// Sets default values
ASevarog::ASevarog()
{
	// 하위에 직접넣는 컴포넌트, Mesh같은거는 여기서
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stat = CreateDefaultSubobject<UAttributeComponent>("Stat");

	// ĳ���� �޽� �ʱ�ȭ
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (SM.Succeeded()) 
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	AIControllerClass = ASevarogAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ASevarog::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<USevarogAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ASevarog::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &ASevarog::AttackCheck);
	}
}

// Called every frame
void ASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SearchInterval -= DeltaTime;

	if (SearchInterval < 0.0f) {
		SearchInterval = 5.0f;
		if (GetCharacterMovement()->MaxWalkSpeed == 0.0f)
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

// Called to bind functionality to input
void ASevarog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASevarog::UpDown(float Value)
{
	UpDownValue = Value;
	AddMovementInput(GetActorForwardVector(), Value);
}

void ASevarog::LeftRight(float Value)
{
	LeftRightValue = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

void ASevarog::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void ASevarog::Attack()
{
	if (IsAttacking)
		return;

	APhase* Target = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	AnimInstance->PlayAttackMontage();
	FVector TargetLocation = Target->GetActorLocation();
	SetActorRotation(TargetLocation.Rotation());
	IsAttacking = true;
	
}

// 공격이 플레이어에게 닿았는지 직접 판단하는 부분
void ASevarog::AttackCheck()
{
	// 거리는 State체크에서 이미 체크했으니 콜라이더 충돌 여부만 판단한다
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRadius = 50.0f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackDist,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 맞은게 확실하다면
	if (bResult && HitResult.GetActor()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 30.f, GetInstigatorController(), this, UDamageType::StaticClass());
		TScriptInterface Interface = TScriptInterface<IHitInterface>(HitResult.GetActor());
		if (Interface)
		{
			Interface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint);
		}
	}
}

void ASevarog::PlayerDieCheck()
{
	if (Player->ActorHasTag("Dead"))
		Player = nullptr;
}

// �߰� ���¿��� ���� ����
void ASevarog::Chase(AActor* Target)
{
	FVector myLocation = GetActorLocation();
	FVector TargetVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	double VectorSize = FVector::Dist(TargetVector, myLocation);

	if (VectorSize > SearchRange)
		State = ESevarogState::E_Idle;

	if (VectorSize < AttackDist)
	{
		//Chase_Attack();
	}

	//FAIMoveRequest MoveRequest;
	//MoveRequest.SetGoalActor(Target);
	//MoveRequest.SetAcceptanceRadius(10.0f);
	//EnemyController->MoveTo(MoveRequest);
}

void ASevarog::Die()
{
	State = ESevarogState::E_Die;
	UE_LOG(LogTemp, Warning, TEXT("State Die"));
}

void ASevarog::StateRefresh()
{
	AnimInstance->StopAllMontages(1.0f);
	UE_LOG(LogTemp, Warning, TEXT("State Refresh"));
	State = ESevarogState::E_Idle;
}


void ASevarog::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsAttacking = false;
	State = ESevarogState::E_Undefine;
	OnAttackEnd.Broadcast();
}

void ASevarog::OnHitMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsHit = false;
	State = ESevarogState::E_Undefine;
	OnHitEnd.Broadcast();
}

void ASevarog::GetHit_Implementation(const FVector& ImpactPoint)
{
	UAnimInstance* Instance = GetMesh()->GetAnimInstance();
	if (Instance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
}

float ASevarog::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->CalculateDamage(DamageAmount);

	return 0.0f;
}

