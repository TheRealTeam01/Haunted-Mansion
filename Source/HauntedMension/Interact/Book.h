// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "Book.generated.h"

class UAnimMontage;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ABook : public AInteract
{
	GENERATED_BODY()
	
public:

	ABook();

	virtual void Interact() override;
	
	UFUNCTION()
	void BookRotate(float DeltaTime);
	
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	void StoneStatueInteract();

	void PlayPullOutAnimation();

private:

	FTimeline Timeline;

	FOnTimelineFloat TimelineUpdate;

	FTimerHandle BookTimer;

	UPROPERTY(EditAnywhere)
		float BookRotation = 120.f;

	UPROPERTY(EditAnywhere)
		float BookLocation = 30.f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UAnimMontage> CharacterAnim;

	UPROPERTY(VisibleAnywhere)
		bool IsRotate = false;

	UPROPERTY(EditAnywhere)
		float WaitTime = 2.f;
};
