// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "SevarogAIController.h"
#include "SevarogAnimInstance.h"

// Sets default values
ASevarog::ASevarog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASevarog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASevarog::UpDown(float Value)
{
	UpDownValue = Value;
}

void ASevarog::LeftRight(float Value)
{
	LeftRightValue = Value;
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
	

}

void ASevarog::Patrol()
{
}

void ASevarog::Chase()
{
}

void ASevarog::Die()
{
}

// 
void ASevarog::Idle_Chase()
{
}

// 이건 패턴을 안정했네요
void ASevarog::Idle_Patrol()
{
}

void ASevarog::Patrol_Chase()
{
}

// 타겟과의 거리를 기준으로 거리내면 무조건 공격
void ASevarog::Chase_Attack()
{

}


void ASevarog::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

