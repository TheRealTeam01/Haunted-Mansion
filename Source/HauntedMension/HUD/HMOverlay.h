// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HMOverlay.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API UHMOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* CrossHair;

	UPROPERTY(meta = (BindWidget))
	UImage* Ammo1;

	UPROPERTY(meta = (BindWidget))
	UImage* Ammo2;

	UPROPERTY(meta = (BindWidget))
	UImage* Ammo3;

	void ShowCrossHair();

	void HideCrossHair();
};
