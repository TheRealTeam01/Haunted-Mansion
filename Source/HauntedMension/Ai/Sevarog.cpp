// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "SevarogAnimInstance.h"
#include "Kismet/GamePlayStatics.h"
#include "Math/Vector.h"

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
}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	EnemyController = Cast<AAIController>(GetController());
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

}

void ASevarog::Attack()
{
	AnimInstance->PlayAttackMontage();
	//State = ESevarogState::E_Idle;
	IsAttacking = true;
	State = ESevarogState::E_Undefine;
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

	UE_LOG(LogTemp, Warning, TEXT("Distance : %f"), VectorSize);
	if (VectorSize > SearchRange) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("State Idle to Patrol"));
		Idle_Patrol();
	}

	if (VectorSize < SearchRange) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("State Idle to Chase"));
		Idle_Chase();
	}
}

// ������ Ư�� ������ ���������� �׳� �ܼ� �̵��Ѵ�
void ASevarog::Patrol()
{
	TArray<AActor*> targetPosition;
	State = ESevarogState::E_Patrol;
}

// �߰� ���¿��� ���� ����
void ASevarog::Chase(AActor* Target)
{
	//UE_LOG(LogTemp, Warning, TEXT("EnemyController is nullptr"));
	//if (EnemyController == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Chase State"));
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
	State = ESevarogState::E_Idle;
	OnAttackEnd.Broadcast();
}

