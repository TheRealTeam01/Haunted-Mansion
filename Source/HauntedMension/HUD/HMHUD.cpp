

#include "HMHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "HMOverlay.h"
#include "HMDeath.h"

void AHMHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	if (PlayerController && HMOverlayClass)
	{
		HMOverlay = CreateWidget<UHMOverlay>(PlayerController, HMOverlayClass);
		HMOverlay->AddToViewport();
	}

	if (PlayerController && HMDeathClass)
	{
		HMDeath = CreateWidget<UHMDeath>(PlayerController, HMDeathClass);
		HMDeath->AddToViewport();
		HMDeath->SetVisibility(ESlateVisibility::Hidden);
	}
}
