#include "HauntedMension/Interact/HintPage.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "HauntedMension/Controller/HMController.h"
#include "HauntedMension/HUD/HMHUD.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

AHintPage::AHintPage()
{
	InteractCamera = CreateDefaultSubobject<UCameraComponent>("Interact Camera");
	InteractCamera->SetupAttachment(Mesh);
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
						if (InteractCamera)
						{
							Controller->SetViewTargetWithBlend(InteractCamera->GetOwner(), CameraBlendTime);
						}

						GetWorld()->GetTimerManager().SetTimer(CameraHandle, [this, Controller, HintPage] () 
							{
								FInputModeUIOnly InputMode;
								InputMode.SetWidgetToFocus(HintPage->TakeWidget());
								Controller->SetInputMode(InputMode);
								HintPage->AddToViewport();
								IsReading = true;
							}
							,CameraBlendTime,
								false
						);
						
					}
				}
			}
		}
	}

}
