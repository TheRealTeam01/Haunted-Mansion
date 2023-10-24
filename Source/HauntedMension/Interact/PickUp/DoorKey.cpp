#include "HauntedMension/Interact/PickUp/DoorKey.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"

ADoorKey::ADoorKey()
{
	Mesh->SetSimulatePhysics(true);
}

void ADoorKey::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->PlayPickUpMontage();
		Character->SetKeyState(EKeyState::EKS_EquippedKey);
		this->Destroy();
	}
}
