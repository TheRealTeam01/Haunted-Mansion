// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "TriggerActor.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ATriggerActor : public AActor
{
	GENERATED_BODY()
	
public:

	ATriggerActor();

	virtual void BeginPlay() override;
	
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;
};
