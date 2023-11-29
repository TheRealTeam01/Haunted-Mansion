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

}

void AHintPage::Interact()
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
				if (TargetCamera)
				{
					Controller->SetViewTargetWithBlend(TargetCamera->GetOwner(), CameraBlendTime);
				}

				GetWorld()->GetTimerManager().SetTimer(CameraHandle, [this, Controller, HintPage]()
					{
						FInputModeUIOnly InputMode;
						InputMode.SetWidgetToFocus(HintPage->TakeWidget());
						Controller->SetInputMode(InputMode);
						HintPage->AddToViewport();
					}
					, CameraBlendTime,
						false
						);
			}
		}
	}
}

