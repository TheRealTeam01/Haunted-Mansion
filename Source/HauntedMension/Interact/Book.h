// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "Book.generated.h"

class UAnimMontage;
class UCurveFloat;
class USoundBase;

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

	/*UFUNCTION()
	void SetPhysics();

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

private:

	FTimeline Timeline;

	FOnTimelineFloat TimelineUpdate;

	FTimerHandle BookTimer;

	/*FOnTimelineEvent TimelineFinish;*/

	UPROPERTY(EditAnywhere)
		float BookRotation = 120.f;

	UPROPERTY(EditAnywhere)
		float BookLocation = 30.f;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UAnimMontage> CharacterAnim;

	/*UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> FallSound;*/

	UPROPERTY(VisibleAnywhere)
		bool IsRotate = false;

	UPROPERTY(EditAnywhere)
		float WaitTime = 2.f;
};
