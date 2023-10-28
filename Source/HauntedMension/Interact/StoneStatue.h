#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "StoneStatue.generated.h"

class UCurveFloat;
class USoundBase;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AStoneStatue : public AInteract
{
	GENERATED_BODY()
	
public:
	
	virtual void Interact() override;

	UFUNCTION()
	void StatueMove(float DeltaTime);

	virtual void Tick(float DeltaTime) override;

	void StatueCameraShake();

protected:

	virtual void BeginPlay() override;

private:

	FTimeline Timeline;

	FOnTimelineFloat TimelineUpdate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MoveSound;

	UPROPERTY(EditAnywhere)
	float MoveValue = 5.f;

	UPROPERTY(EditAnywhere)
		bool IsMove = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShake;
};
