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

	// 캐릭터 메시 초기화
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
	Player->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("%s"), Player);
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
		Chase();
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
	// 몬스터가 행하는 공격에 대한 내용이 들어간다.
	if (IsAttacking)
		return;

	// 여기서 애니메이션 인스턴스와 연결한다.
	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 3;

	IsAttacking = true;
}

void ASevarog::AttackCheck()
{
	//피격 판정과 관련된 내용이 들어간다.
	FHitResult HitResult;
	// 충돌 체크를 위한 충돌 파라미터 지정
	FCollisionQueryParams Params(NAME_None, false, this);

	// 피격판정인데 뭘로 공격하는지를 모르겠다
}


void ASevarog::Idle()
{
	UE_LOG(LogTemp, Warning, TEXT("Player now State is Idle"));
	double SearchRange = 100.f;
	double PatrolRange = 50.f;

	// 우선 거리를 체크한다
	FVector myLocation = GetActorLocation();
	// 플레어이를 찾아와서 
	FVector TargetVector = Player->GetTargetLocation();

	// 여기서 우선 플레이어가 있는지 체크를
	// 여기서 null 뜬다
	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Distance : %s"), TargetVector);
	// 플레이어가 서치 범위내에 없다면 Patrol로 상태 전이
	if (FVector::Dist(myLocation, TargetVector) < SearchRange) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Idle to Patrol"))
		State = ESevarogState::E_Patrol;
	}

	// 플레이어가 서치 범위 내에 있다면 Chase 상태로 전이
	if (FVector::Dist(myLocation, TargetVector) < PatrolRange) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Idle to Chase"));
		State = ESevarogState::E_Chase;
	}
}

// 순찰은 특정 지점과 지점까지를 그냥 단순 이동한다
void ASevarog::Patrol()
{
	// 랜덤 좌표를 지정해서 처리해야한다. 근데 NavMesh가 없는데? 어케하지?
}

// 추격 상태에서 행할 내용
void ASevarog::Chase()
{
	FVector myLocation = GetActorLocation();
	FVector TargetVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	if (FVector::Dist(myLocation, TargetVector) < AttackDist)
	{
		Chase_Attack();
	}

	
}

// 이건 안쓸거같은데요 일단 넣어놨습니다
void ASevarog::Die()
{
	State = ESevarogState::E_Idle;
}

// Idle에서 Chase로 상태전이 -> Heard상태이거나, 근처에 있음을 감지
void ASevarog::Idle_Chase()
{
	State = ESevarogState::E_Chase;
}

// 이건 패턴을 안정했네요
void ASevarog::Idle_Patrol()
{
	State = ESevarogState::E_Patrol;
}

void ASevarog::Patrol_Chase()
{
	State = ESevarogState::E_Chase;
}

// 타겟과의 거리를 기준으로 거리내면 무조건 공격
void ASevarog::Chase_Attack()
{
	State = ESevarogState::E_Attack;
}


void ASevarog::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

