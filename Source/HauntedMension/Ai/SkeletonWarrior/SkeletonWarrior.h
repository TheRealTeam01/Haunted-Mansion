#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Components/TimelineComponent.h"
#include "SkeletonWarrior.generated.h"

class UAttributeComponent;
class UPawnSensingComponent;
class UBehaviorTree;

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
		UBehaviorTree* BehaviorTree;
protected:

	virtual void BeginPlay() override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UFUNCTION()
	void Chase(APawn* PawnInstigator, const FVector& Location, float Volume);

	void Die();

	UPROPERTY(EditAnywhere)
		UAnimMontage* ScreamMontage;
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* DieMontage;

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
		
	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere)
		bool IsStanding = false;
	
	UPROPERTY(VisibleAnywhere)
		AActor* PlayerCharacter = nullptr;


public:	
	
};
