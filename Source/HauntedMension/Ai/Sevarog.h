// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HauntedMension/HMTypes/HMTypes.h"
#include "Sevarog.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class HAUNTEDMENSION_API ASevarog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASevarog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���� �̵� ���ݵ� ���� �Լ�
	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);

	FOnAttackEnd OnAttackEnd;

	// ����Ƽ ����ó�� �ν��Ͻ� ��ŸƮ�� ����
	// ���� ���ݿ� ���� �Լ�
	void Attack();
	// ���� �ǰ� ������ ���� �Լ�
	void AttackCheck();
	
	// ���� ���� üũ�� ���� �ൿ ����
	void Idle();
	void Patrol();
	void Chase(AActor* Target);
	void Die();

	// ���� ���� �Լ�
	void Idle_Chase();
	void Idle_Patrol();
	void Patrol_Chase();
	void Chase_Attack();

	// ���¸ӽ��� AIController���� ó���Ѵ�.
	// ���ݻ��°� ����Ǹ� ��������Ʈ�� ���� �˸���.
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted);

private:
	// ���� ���¸� �˱� ���� �÷���
	UPROPERTY()
	bool IsAttacking = false;

	UPROPERTY()
	float AttackDist = 10.0f;

	UPROPERTY()
	class USevarogAnimInstance* AnimInstance;

	UPROPERTY()
	int32 AttackIndex = 0;

public:
	UPROPERTY()
	float UpDownValue;

	UPROPERTY()
	float LeftRightValue;

	UPROPERTY(VisibleAnywhere)
	ESevarogState State;

	UPROPERTY(VisibleAnywhere)
	AActor* Player;
};
