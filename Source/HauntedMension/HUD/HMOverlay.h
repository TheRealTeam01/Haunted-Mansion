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
	class UImage* CrossHairCenter;

	UPROPERTY(meta = (BindWidget))
		UImage* CrossHairTop;

	UPROPERTY(meta = (BindWidget))
		UImage* CrossHairBottom;

	UPROPERTY(meta = (BindWidget))
		UImage* CrossHairRight;

	UPROPERTY(meta = (BindWidget))
		UImage* CrossHairLeft;
	
	UPROPERTY(meta = (BindWidget))
		UImage* BloodSplatter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CarriedAmmoText;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NoAmmoText;

	UPROPERTY(meta = (BindWidgetAnim), transient)
		class UWidgetAnimation* Blink;
	
	UPROPERTY(meta = (BindWidgetAnim), transient)
		UWidgetAnimation* HP80;
	
	void ShowCrossHair();

	void HideCrossHair();

	void PlayBlink();

	void ShowBloodSplatter(float Health);
};
