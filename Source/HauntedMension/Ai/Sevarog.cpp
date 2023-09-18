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

// �̰� ������ �����߳׿�
void ASevarog::Idle_Patrol()
{
}

void ASevarog::Patrol_Chase()
{
}

// Ÿ�ٰ��� �Ÿ��� �������� �Ÿ����� ������ ����
void ASevarog::Chase_Attack()
{

}


void ASevarog::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

