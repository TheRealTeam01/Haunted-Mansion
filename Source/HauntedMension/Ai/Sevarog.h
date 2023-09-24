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

	// 몬스터 이동 공격등 상태 함수
	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);

	FOnAttackEnd OnAttackEnd;

	// 유니티 포톤처럼 인스턴스 스타트는 생략
	// 몬스터 공격에 관한 함수
	void Attack();
	// 몬스터 피격 판정에 관한 함수
	void AttackCheck();
	
	// 몬스터 상태 체크에 따른 행동 매핑
	void Idle();
	void Patrol();
	void Chase();
	void Die();

	// 상태 전이 함수
	void Idle_Chase();
	void Idle_Patrol();
	void Patrol_Chase();
	void Chase_Attack();

	// 상태머신은 AIController에서 처리한다.
	// 공격상태가 종료되면 델리게이트를 통해 알린다.
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted);

private:
	// 공격 상태를 알기 위한 플래그
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
	ACharacter* Player;
};
