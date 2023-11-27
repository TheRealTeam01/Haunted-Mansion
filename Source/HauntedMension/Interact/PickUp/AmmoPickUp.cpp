#include "AmmoPickUp.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

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
	if (Character && Character->GetDefaultWeapon())
	{
		Character->PlayPickUpMontage();
		Character->GetDefaultWeapon()->PickUpAmmo(Character->GetDefaultWeapon()->AmmountToPickUp);
		this->Destroy();
	}
}

