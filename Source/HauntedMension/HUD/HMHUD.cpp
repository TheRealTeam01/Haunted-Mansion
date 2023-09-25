

#include "HMHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

void AHMHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	if (PlayerController && CrossHairClasses)
	{
		CrossHairWidget = CreateWidget<UUserWidget>(PlayerController, CrossHairClasses);
	}
}

void AHMHUD::ShowCrossHair()
{
	if (CrossHairWidget)
	{
		CrossHairWidget->AddToViewport();
	}
}

void AHMHUD::HideCrossHair()
{
	if (CrossHairWidget)
	{
		CrossHairWidget->RemoveFromParent();
	}
}
