#include "HauntedMension/Interact/PickUp/Ammo.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

AAmmo::AAmmo()
{
}


void AAmmo::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character && Character->GetDefaultWeapon())
	{
		Character->PlayPickUpMontage();
		Character->GetDefaultWeapon()->PickUpAmmo(Character->GetDefaultWeapon()->AmmountToPickUp);
		this->Destroy();
	}
}
