// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "SevarogAnimInstance.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Ai/SevarogActorComponent.h"
#include "HauntedMension/Attribute/AttributeComponent.h"

// Sets default values
ASevarog::ASevarog()
{
	// 하위에 직접넣는 컴포넌트, Mesh같은거는 여기서
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���� �޽� �ʱ�ȭ
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (SM.Succeeded()) 
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	Stat = CreateDefaultSubobject<UAttributeComponent>("Attributes");
}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	EnemyController = Cast<AAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	
	//UE_LOG(LogTemp, Warning, TEXT("Player Actor Name : %s"), Player->GetFName());
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
	float Count = FApp::GetDeltaTime();
	SearchInterval -= Count;

	switch (State) 
	{
	case ESevarogState::E_Idle:
		Idle();
		break;
	case ESevarogState::E_Patrol:
		if (SearchInterval <= 0.0f)
			Patrol();
		break;
	case ESevarogState::E_Chase:
		Chase(Player);
		break;
	case ESevarogState::E_Attack:
		Attack();
		break;
	default:
		StateRefresh();
		break;
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

	AnimInstance->PlayAttackMontage();
	GetCharacterMovement()->MaxWalkSpeed = 1.0f;

	//State = ESevarogState::E_Idle;
	IsAttacking = true;
	State = ESevarogState::E_Undefine;
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

	FColor DrawColor;
	FVector Vec = GetActorForwardVector() * AttackDist;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	float HalfHeight = AttackDist * 0.5f + AttackRadius;

	// 공격 이벤트가 실행되는 지점을 알기 위한 DrawShape
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		Rotation, DrawColor, false, 2.0f);

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
		//FDamageEvent DamageEvent;
	}
}


void ASevarog::Idle()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	// �켱 �Ÿ��� üũ�Ѵ�
	FVector myLocation = GetActorLocation();
	// �÷����̸� ã�ƿͼ� 
	FVector TargetVector = Player->GetActorLocation();
	FVector Distance = TargetVector - myLocation;
	float VectorSize = Distance.Size();

	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is nullptr"));
		return;
	}

	if (VectorSize > SearchRange) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("State Idle to Patrol"));
		Idle_Patrol();
	}

	else if (VectorSize < SearchRange) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("State Idle to Chase"));
		Idle_Chase();
	}
}

// ������ Ư�� ������ ���������� �׳� �ܼ� �̵��Ѵ�
void ASevarog::Patrol()
{
	FVector PlayerVector = Player->GetActorLocation();
	FVector MyVector = GetActorLocation();
	FVector DistVector = PlayerVector - MyVector;

	FQuat Dir = DistVector.ToOrientationQuat();

	float DistSize = DistVector.Size();
	if (DistSize < SearchRange)
		ESevarogState::E_Undefine;

	FVector GoalLocation;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 1500.f, RandomLocation)) {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(EnemyController, RandomLocation);
	}
	SearchInterval = 5.0f;
	State = ESevarogState::E_Undefine;
}

// �߰� ���¿��� ���� ����
void ASevarog::Chase(AActor* Target)
{
	FVector myLocation = GetActorLocation();
	FVector TargetVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	double VectorSize = FVector::Dist(TargetVector, myLocation);

	if (VectorSize > SearchRange)
		ESevarogState::E_Idle;

	if (VectorSize < AttackDist)
	{
		Chase_Attack();
	}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(10.0f);
	EnemyController->MoveTo(MoveRequest);
}

void ASevarog::Die()
{
	State = ESevarogState::E_Die;
	UE_LOG(LogTemp, Warning, TEXT("State Die"));
}

void ASevarog::StateRefresh()
{
	UE_LOG(LogTemp, Warning, TEXT("State Refresh"));
	State = ESevarogState::E_Idle;
}

void ASevarog::Idle_Chase()
{	
	State = ESevarogState::E_Chase;
}

// 여기서 가장 가까운 지점을 정찰하도록 위치를 지정해준다
void ASevarog::Idle_Patrol()
{
	State = ESevarogState::E_Patrol;
	//UE_LOG(LogTemp, Warning, TEXT("State Idle to Patrol"));
}

void ASevarog::Patrol_Chase()
{
	State = ESevarogState::E_Chase;
	//UE_LOG(LogTemp, Warning, TEXT("State Patrol to Chase"));
}

// Ÿ�ٰ��� �Ÿ��� �������� �Ÿ����� ������ ����
void ASevarog::Chase_Attack()
{
	State = ESevarogState::E_Attack;
	//UE_LOG(LogTemp, Warning, TEXT("State Chase to Attack"));
}


void ASevarog::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsAttacking = false;
	State = ESevarogState::E_Undefine;
	OnAttackEnd.Broadcast();
}

void ASevarog::GetHit_Implementation(const FVector& ImpactPoint)
{
	FVector Forward = GetActorForwardVector();
	FVector ToTarget = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("Sevarog GetHit Implmentation"));

	if (Stat) 
	{
		UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
		if (Stat->IsDead() && DeathMontage)
		{
			AnimIns->Montage_Play(DeathMontage);
			State = ESevarogState::E_Die;
			GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		else 
		{
			if (AnimIns && HitMontage)
			{
				AnimIns->Montage_Play(HitMontage);
			}
		}
	}
}

float ASevarog::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

