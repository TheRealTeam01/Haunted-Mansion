// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HMController.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AHMController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void SetHUDAmmo(int32 Ammo);

	void SetHUDCarriedAmmo(int32 CarriedAmmo);

	void SetHUDHealth(float HealthPercent);

	void SetHUDStamina(float StaminaPercent);

	void SetHUDDie();

};
