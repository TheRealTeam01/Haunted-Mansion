
#include "HMController.h"
#include "HauntedMension/HUD/HMOverlay.h"
#include "HauntedMension/HUD/HMHUD.h"
#include "HauntedMension/HUD/HMDeath.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void AHMController::SetHUDAmmo(int32 Ammo)
{
	AHMHUD* HMHUD = Cast<AHMHUD>(GetHUD());

	bool bValidHUD = HMHUD &&
		HMHUD->HMOverlay &&
		HMHUD->HMOverlay->AmmoText;
	
	if (bValidHUD)
	{
		HMHUD->HMOverlay->AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Ammo)));
	}
}

void AHMController::SetHUDCarriedAmmo(int32 CarriedAmmo)
{
	AHMHUD* HMHUD = Cast<AHMHUD>(GetHUD());

	bool bValidHUD = HMHUD &&
		HMHUD->HMOverlay &&
		HMHUD->HMOverlay->CarriedAmmoText;

	if (bValidHUD)
	{
		HMHUD->HMOverlay->CarriedAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CarriedAmmo)));
	}
}

void AHMController::SetHUDHealth(float HealthPercent)
{
	AHMHUD* HMHUD = Cast<AHMHUD>(GetHUD());

	bool bValidHUD = HMHUD &&
		HMHUD->HMOverlay &&
		HMHUD->HMOverlay->HPBar;
}

void AHMController::SetHUDStamina(float StaminaPercent)
{
	AHMHUD* HMHUD = Cast<AHMHUD>(GetHUD());
	
	bool bValidHUD = HMHUD &&
		HMHUD->HMOverlay &&
		HMHUD->HMOverlay->StaminaBar;

	if (bValidHUD)
	{
		HMHUD->HMOverlay->StaminaBar->SetPercent(StaminaPercent);
	}
}

void AHMController::SetHUDDie()
{
	AHMHUD* HMHUD = Cast<AHMHUD>(GetHUD());

	bool bValidHUD = HMHUD &&
		HMHUD->HMDeath &&
		HMHUD->HMDeath->DeathText &&
		HMHUD->HMDeath->BloodSplatter &&
		HMHUD->HMDeath->DeathAnim;

	if (bValidHUD)
	{
		HMHUD->HMOverlay->RemoveFromParent();
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
		HMHUD->HMDeath->SetVisibility(ESlateVisibility::Visible);
		HMHUD->HMDeath->PlayDeathAnim();
	}
}
