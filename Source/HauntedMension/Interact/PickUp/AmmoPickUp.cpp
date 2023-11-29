#include "AmmoPickUp.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Controller/HMController.h"
#include "HauntedMension/HUD/HMOverlay.h"
#include "HauntedMension/HUD/HMHUD.h"

AAmmoPickUp::AAmmoPickUp()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
		
	/*Mesh->SetCustomDepthStencilValue(252);
	Mesh->MarkRenderStateDirty();
	EnableCustomDepth(true);*/
}

void AAmmoPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPickUp::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));	
	AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Character && Controller && Character->GetDefaultWeapon())
	{
		Controller->GetHUD<AHMHUD>()->HMOverlay->PlayPickUp(FText::FromString("+ 5"),AmmoImage);
		Character->PlayPickUpMontage();
		Character->GetDefaultWeapon()->PickUpAmmo(Character->GetDefaultWeapon()->AmmountToPickUp);
		this->Destroy();
	}
}

