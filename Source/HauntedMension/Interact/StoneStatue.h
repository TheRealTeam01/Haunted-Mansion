#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "StoneStatue.generated.h"

class UCurveFloat;
class USoundBase;
class UParticleSystem;
class UParticleSystemComponent;

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

	FTimeline Timeline;

	FOnTimelineFloat TimelineUpdate;

	FOnTimelineEvent TimelineFinish;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MoveSound;

	FTimerHandle MoveEffectHandle;

	UPROPERTY(EditAnywhere)
	float MoveValue = 5.f;

	UPROPERTY(EditAnywhere)
		bool IsMove = false;

	UPROPERTY(EditAnywhere)
		float EffectTime = 1.f;

	UPROPERTY(EditAnywhere)
		float ZLocation = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> MoveEffect;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UParticleSystemComponent> MoveEffectComponent;
};
