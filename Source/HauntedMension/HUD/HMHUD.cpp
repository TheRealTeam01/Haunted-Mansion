

#include "HMHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "HMOverlay.h"
#include "HMDeath.h"
#include "HauntedMension/Controller/HMController.h"

void AHMHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	AHMController* HMController = Cast<AHMController>(GetWorld()->GetFirstPlayerController());
	if (HMController)
	{
		if (HMOverlayClass)
		{
			HMOverlay = CreateWidget<UHMOverlay>(HMController, HMOverlayClass);
			HMOverlay->AddToViewport();
		}

		if (HMDeathClass)
		{
			HMDeath = CreateWidget<UHMDeath>(HMController, HMDeathClass);
			HMDeath->AddToViewport();
			HMDeath->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}
