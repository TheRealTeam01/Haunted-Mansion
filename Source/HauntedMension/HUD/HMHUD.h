// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HMHUD.generated.h"

class UHMOverlay;
class UHMDeath;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AHMHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void PreInitializeComponents() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UHMOverlay> HMOverlayClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UHMDeath> HMDeathClass;

	UHMOverlay* HMOverlay;

	UHMDeath* HMDeath;

	FORCEINLINE UHMOverlay* GetHMOverlay() { return HMOverlay; }

protected:




};
