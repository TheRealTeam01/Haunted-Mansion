#include "HauntedMension/Interact/KeyPad.h"
#include "Camera/CameraComponent.h"
#include "KeyActor.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Controller/HMController.h"
#include "Kismet/KismetStringLibrary.h"

AKeyPad::AKeyPad()
{	
	InteractCamera = CreateDefaultSubobject<UCameraComponent>("InteractCamera");
	InteractCamera->SetupAttachment(Mesh);
	EnterCodeText = CreateDefaultSubobject<UTextRenderComponent>("EnterCodeText");
	EnterCodeText->SetupAttachment(Mesh);
	Buttons = CreateDefaultSubobject<USceneComponent>("Buttons");
	Buttons->SetupAttachment(Mesh);
	KeyActor1 = CreateDefaultSubobject<UChildActorComponent>("Key1");
	KeyActor1->SetupAttachment(Buttons);
	KeyActor2 = CreateDefaultSubobject<UChildActorComponent>("Key2");
	KeyActor2->SetupAttachment(Buttons);
	KeyActor3 = CreateDefaultSubobject<UChildActorComponent>("Key3");
	KeyActor3->SetupAttachment(Buttons);
	KeyActor4 = CreateDefaultSubobject<UChildActorComponent>("Key4");
	KeyActor4->SetupAttachment(Buttons);
	KeyActor5 = CreateDefaultSubobject<UChildActorComponent>("Key5");
	KeyActor5->SetupAttachment(Buttons);
	KeyActor6 = CreateDefaultSubobject<UChildActorComponent>("Key6");
	KeyActor6->SetupAttachment(Buttons);
	KeyActor7 = CreateDefaultSubobject<UChildActorComponent>("Key7");
	KeyActor7->SetupAttachment(Buttons);
	KeyActor8 = CreateDefaultSubobject<UChildActorComponent>("Key8");
	KeyActor8->SetupAttachment(Buttons);
	KeyActor9 = CreateDefaultSubobject<UChildActorComponent>("Key9");
	KeyActor9->SetupAttachment(Buttons);
	KeyActor0 = CreateDefaultSubobject<UChildActorComponent>("Key0");
	KeyActor0->SetupAttachment(Buttons);
	BackspaceKey = CreateDefaultSubobject<UChildActorComponent>("Backspace");
	BackspaceKey->SetupAttachment(Buttons);
	EnterKey = CreateDefaultSubobject<UChildActorComponent>("Enter");
	EnterKey->SetupAttachment(Buttons);

}

void AKeyPad::BeginPlay()
{	
	Super::BeginPlay();
}

void AKeyPad::Interact()
{
	if (!bCanEnter)
	{
		AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		check(Controller);

		Controller->SetViewTargetWithBlend(InteractCamera->GetOwner(), CameraBlendTime);
		Initiate();
	}
}

void AKeyPad::Initiate()
{
	AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(Controller);

	Controller->SetShowMouseCursor(true);
	Controller->bEnableClickEvents = true;
	Controller->bEnableMouseOverEvents = true;

	FString InitialCodeString = FString("");
	FText InitialCode = FText::FromString(InitialCodeString);
	EnterCodeText->SetText(InitialCode);

	ButtonArray.Add(KeyActor1);
	ButtonArray.Add(KeyActor2);
	ButtonArray.Add(KeyActor3);
	ButtonArray.Add(KeyActor4);
	ButtonArray.Add(KeyActor5);
	ButtonArray.Add(KeyActor6);
	ButtonArray.Add(KeyActor7);
	ButtonArray.Add(KeyActor8);
	ButtonArray.Add(KeyActor9);
	ButtonArray.Add(KeyActor0);
	ButtonArray.Add(BackspaceKey);
	ButtonArray.Add(EnterKey);


}
