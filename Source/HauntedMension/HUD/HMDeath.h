// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HMDeath.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API UHMDeath : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void PlayDeathAnim();

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> DeathText;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UImage> BloodSplatter;

	UPROPERTY(meta = (BindWidgetAnim), transient)
		TObjectPtr<UWidgetAnimation> DeathAnim;

protected:

};
