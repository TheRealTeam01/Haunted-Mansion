// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "Book.generated.h"

class UAnimMontage;
class UCurveFloat;
class USoundBase;
class UCameraComponent;
class ULevelSequencePlayer;
class ULevelSequence;

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
	void BookMove(float DeltaTime);
	
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void StoneStatueInteract();

	void PlayPullOutAnimation();

	UFUNCTION()
	void SetPhysics();

	//UFUNCTION()
	//	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
		TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelSequence> LevelSequence;

	TObjectPtr<UTimelineComponent> Timeline;

	FOnTimelineFloat TimelineUpdate;

	FTimerHandle BookTimer;

	FOnTimelineEvent TimelineFinish;

	UPROPERTY(EditAnywhere)
		float LocationX = 0.f;

	UPROPERTY(EditAnywhere)
		float LocationY = 0.f;

	UPROPERTY(EditAnywhere)
		float LocationZ = 0.f;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> StoneStatueClass;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UAnimMontage> CharacterAnim;

	/*UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> FallSound;*/

	UPROPERTY(EditAnywhere)
		float WaitTime = 2.f;
private:

};
