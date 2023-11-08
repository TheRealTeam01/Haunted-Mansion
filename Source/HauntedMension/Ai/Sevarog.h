// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/TimeLineComponent.h"
#include "HauntedMension/HMTypes/HMTypes.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Sevarog.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnHitEnd);

UCLASS()
class HAUNTEDMENSION_API ASevarog : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASevarog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);

	FOnAttackEnd OnAttackEnd;
	FOnHitEnd OnHitEnd;

	void Attack();
	void AttackCheck();
	
	// ���� ���� üũ�� ���� �ൿ ����
	void Idle();
	void Patrol();
	void Chase(AActor* Target);
	
	UFUNCTION()
	void Die();
	void StateRefresh();

	// ���� ���� �Լ�
	void Idle_Chase();
	void Idle_Patrol();
	void Patrol_Chase();
	void Chase_Attack();
	// ���¸ӽ��� AIController���� ó���Ѵ�.
	// ���ݻ��°� ����Ǹ� ��������Ʈ�� ���� �˸���.
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruppted);
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterruppted);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;

	FOnTimelineFloat DissolveTimelineUpdate;

	FOnTimelineEvent DissolveTimelineFinished;

	UFUNCTION()
	void StartDissolve();

	UFUNCTION()
	void StopDissolve();

	UFUNCTION()
	void UpdateDissolve(float DeltaTime);

	UPROPERTY(EditAnywhere)
		UCurveFloat* DissolveCurve;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* DissolveMateialInstance;
private:
	// ���� ���¸� �˱� ���� �÷���
	UPROPERTY()
	bool IsAttacking = false;

	UPROPERTY()
	bool IsHit = false;

	UPROPERTY()
	float AttackDist = 150.0f;

	UPROPERTY()
	float SearchRange = 500.0f;

	UPROPERTY()
	float SearchInterval;

	UPROPERTY()
	class USevarogAnimInstance* AnimInstance;

	UPROPERTY()
	int32 AttackIndex = 0;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Stat;

public:
	UPROPERTY()
	float UpDownValue;

	UPROPERTY()
	float LeftRightValue;

	UPROPERTY(VisibleAnywhere)
	ESevarogState State;

	UPROPERTY(VisibleAnywhere)
	AActor* Player;

	UPROPERTY(VisibleAnywhere)
	AActor* PatrolTarget;

	UPROPERTY(VisibleAnywhere)
	AAIController* EnemyController;

	UPROPERTY(EditAnywhere)
	 class UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;
};
