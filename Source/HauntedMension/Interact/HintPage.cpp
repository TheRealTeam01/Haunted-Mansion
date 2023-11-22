#include "HauntedMension/Interact/HintPage.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "HauntedMension/Controller/HMController.h"
#include "HauntedMension/HUD/HMHUD.h"
#include "Gameframework/CharacterMovementComponent.h"

AHintPage::AHintPage()
{

}

void AHintPage::Interact()
{
	if (!IsReading)
	{
		AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Controller)
		{
			UUserWidget* HintPage = CreateWidget<UUserWidget>(Controller, HintPageWidget);
			if (HintPage)
			{
				APhase* Player = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (Player)
				{
					if (!IsReading)
					{
						Player->GetCharacterMovement()->DisableMovement();
						HintPage->AddToViewport();
						IsReading = true;
					}
					else
					{
						Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
						HintPage->RemoveFromParent();
						IsReading = false;
					}
				}
			}
		}
	}

}
