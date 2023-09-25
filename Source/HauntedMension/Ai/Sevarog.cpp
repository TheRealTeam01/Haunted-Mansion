// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "SevarogAIController.h"
#include "SevarogAnimInstance.h"
#include "Kismet/GamePlayStatics.h"
#include "Math/Vector.h"


// Sets default values
ASevarog::ASevarog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���� �޽� �ʱ�ȭ
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (SM.Succeeded()) 
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//UE_LOG(LogTemp, Warning, TEXT("Player Actor Name : %s"), Player->GetFName());
}

// Called every frame
void ASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (State) 
	{
	case ESevarogState::E_Idle:
		Idle();
		break;
	case ESevarogState::E_Patrol:
		Patrol();
		break;
	case ESevarogState::E_Chase:
		Chase(Player);
		break;
	case ESevarogState::E_Attack:
		Attack();
		break;

	default:
		Idle();
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

}

void ASevarog::Attack()
{
	// ���Ͱ� ���ϴ� ���ݿ� ���� ������ ����.
	if (IsAttacking)
		return;

	// ���⼭ �ִϸ��̼� �ν��Ͻ��� �����Ѵ�.
	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 3;

	IsAttacking = true;
}

void ASevarog::AttackCheck()
{
	//�ǰ� ������ ���õ� ������ ����.
	FHitResult HitResult;
	// �浹 üũ�� ���� �浹 �Ķ���� ����
	FCollisionQueryParams Params(NAME_None, false, this);

	// �ǰ������ε� ���� �����ϴ����� �𸣰ڴ�
}


void ASevarog::Idle()
{
	//UE_LOG(LogTemp, Warning, TEXT("Sevarog now State is Idle"));
	float SearchRange = 1000.f;
	float PatrolRange = 500.f;

	// �켱 �Ÿ��� üũ�Ѵ�
	FVector myLocation = GetActorLocation();
	// �÷����̸� ã�ƿͼ� 
	FVector TargetVector = Player->GetActorLocation();
	FVector Distance = TargetVector - myLocation;
	float VectorSize = Distance.Size();

	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player is nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Distance : %f"), VectorSize);
	if (FVector::Dist(myLocation, TargetVector) < SearchRange) 
	{
		Idle_Patrol();
	}

	if (FVector::Dist(myLocation, TargetVector) < PatrolRange) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Idle to Chase"));
		Idle_Chase();
	}
}

// ������ Ư�� ������ ���������� �׳� �ܼ� �̵��Ѵ�
void ASevarog::Patrol()
{
	State = ESevarogState::E_Patrol;
}

// �߰� ���¿��� ���� ����
void ASevarog::Chase(AActor* Target)
{
	FVector myLocation = GetActorLocation();
	FVector TargetVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	if (FVector::Dist(myLocation, TargetVector) < AttackDist)
	{
		Chase_Attack();
	}

	
}

void ASevarog::Die()
{
	State = ESevarogState::E_Die;
	UE_LOG(LogTemp, Warning, TEXT("State Die"));
}

void ASevarog::Idle_Chase()
{
	State = ESevarogState::E_Chase;
	UE_LOG(LogTemp, Warning, TEXT("State Idle to Chase"));
}

// �̰� ������ �����߳׿�
void ASevarog::Idle_Patrol()
{
	State = ESevarogState::E_Patrol;
	UE_LOG(LogTemp, Warning, TEXT("State Idle to Patrol"));
}

void ASevarog::Patrol_Chase()
{
	State = ESevarogState::E_Chase;
	UE_LOG(LogTemp, Warning, TEXT("State Patrol to Chase"));
}

// Ÿ�ٰ��� �Ÿ��� �������� �Ÿ����� ������ ����
void ASevarog::Chase_Attack()
{
	State = ESevarogState::E_Attack;
	UE_LOG(LogTemp, Warning, TEXT("State Chase to Attack"));
}


void ASevarog::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

