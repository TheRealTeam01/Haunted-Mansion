#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "StoneStatue.generated.h"

class UCurveFloat;
class USoundBase;
class UParticleSystem;
class UParticleSystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AStoneStatue : public AInteract
{
	GENERATED_BODY()
	
public:
	
	AStoneStatue();

	virtual void Interact() override;

	UFUNCTION()
	void StatueMove(float DeltaTime);

	virtual void Tick(float DeltaTime) override;

	void PlayCameraShake();

	void SpawnMoveEffect();

	UFUNCTION()
	void DestroyMoveEffect();

protected:

	virtual void BeginPlay() override;


private:

	TObjectPtr<UTimelineComponent> Timeline;

	FOnTimelineFloat TimelineUpdate;

	FOnTimelineEvent TimelineFinish;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MoveSound;

	FTimerHandle MoveEffectHandle;

	UPROPERTY(EditAnywhere)
	float LocationX = 0.f;
	
	UPROPERTY(EditAnywhere)
	float LocationY = 0.f;
	
	UPROPERTY(EditAnywhere)
	float LocationZ= 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> MoveEffect;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UParticleSystemComponent> MoveEffectComponent;
};
