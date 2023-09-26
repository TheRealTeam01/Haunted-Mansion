// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HMHUD.generated.h"

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
		TSubclassOf<class UHMOverlay> HMOverlayClass;

	UHMOverlay* HMOverlay;

	APlayerController* PlayerController;

	FORCEINLINE UHMOverlay* GetHMOverlay() { return HMOverlay; }

};
