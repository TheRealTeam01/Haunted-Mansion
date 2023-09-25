

#include "HMHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "HMOverlay.h"

void AHMHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	if (PlayerController && HMOverlayClass)
	{
		HMOverlay = CreateWidget<UHMOverlay>(PlayerController, HMOverlayClass);
		HMOverlay->AddToViewport();
	}
}
