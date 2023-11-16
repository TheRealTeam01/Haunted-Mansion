#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Components/TimelineComponent.h"
#include "SkeletonWarrior.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnded);

class UAttributeComponent;
class UPawnSensingComponent;
class UBehaviorTree;
class USphereComponent;

UCLASS()
class HAUNTEDMENSION_API ASkeletonWarrior : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	
	ASkeletonWarrior();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void StartDissolve();

	UFUNCTION()
		void StopDissolve();

	UFUNCTION()
		void UpdateDissolve(float DeltaTime);

	UPROPERTY(EditAnywhere)
		bool IsStanding = false;

	void Attack();

	FOnAttackEnded OnAttackEnded;

protected:

	virtual void BeginPlay() override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;;
	
	void InitializeAIComponents();

	UFUNCTION()
		void UpdateDie(float DeltaTime);

	void Die();

	void StandUp();

	UFUNCTION(BlueprintCallable)
	void AttackTrace(USphereComponent* HitBox);

	UPROPERTY(EditAnywhere)
		float TraceDistance = 100.f;

	UPROPERTY(EditAnywhere)
		float TraceRadius = 50.f;

	UPROPERTY(EditAnywhere)
		float Damage = 30.f;

	UPROPERTY(EditAnywhere)
		bool ShowDebugBox = false;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ScreamMontage;
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* DieMontage;
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* StandUpMontage;
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* AttackMontage;

	/* Dissolve Effect */

	UPROPERTY(VisibleAnywhere)
		UTimelineComponent* DissolveTimeline;

	FOnTimelineFloat DissolveTimelineUpdate;

	FOnTimelineEvent DissolveTimelineFinished;

	UPROPERTY(EditAnywhere)
		UCurveFloat* DissolveCurve;
	
	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* DissolveMateialInstance;

	/* Die Effect */

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DieTimeline;

	FOnTimelineFloat DieTimelineUpdate;

	FOnTimelineEvent DieTimelineFinished;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DieCurve;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicDieMaterialInstance;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* DieMateialInstance;

	FTimerHandle ScreamHandle;

	/* Basic Properites */

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attribute;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* RightHandSphere;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* LeftHandSphere;

	UPROPERTY(VisibleAnywhere)
		bool IsAttacking = false;

	/* AI */

	UPROPERTY(EditAnywhere)
		UBehaviorTree* BehaviorTree;

	UPROPERTY()
		class ASkeletonWarriorAIController* AIController;

public:	
	
	FORCEINLINE bool SetIsStanding(bool bStanding) { return IsStanding = bStanding; }
	FORCEINLINE bool GetIsStanding() { return IsStanding; }
};
