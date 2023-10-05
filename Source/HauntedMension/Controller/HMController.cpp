
#include "HMController.h"
#include "HauntedMension/HUD/HMOverlay.h"
#include "HauntedMension/HUD/HMHUD.h"
#include "Components/TextBlock.h"

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

void AHMController::SetHUDHealth(float Health)
{
	AHMHUD* HMHUD = Cast<AHMHUD>(GetHUD());

	bool bValidHUD = HMHUD &&
		HMHUD->HMOverlay;

	if (bValidHUD)
	{
		HMHUD->HMOverlay->ShowBloodSplatter(Health);
	}
}
