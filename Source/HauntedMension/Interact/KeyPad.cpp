#include "HauntedMension/Interact/KeyPad.h"
#include "Camera/CameraComponent.h"
#include "KeyActor.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Controller/HMController.h"
#include "Kismet/KismetStringLibrary.h"
#include "HauntedMension/Character/Phase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Character/Phase.h"
#include "GameframeWork/CharacterMovementComponent.h"
AKeyPad::AKeyPad()
{	
	EnterPassword = CreateDefaultSubobject<UTextRenderComponent>("EnterCodeText");
	EnterPassword->SetupAttachment(Mesh);
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
	AInteract* InteractItem = Cast<AInteract>(UGameplayStatics::GetActorOfClass(this, InteractActor));
	InteractItem->OnInteractEnded.AddDynamic(this, &AKeyPad::SetViewPlayerCamera);

	if (AttachActorClass)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
		this->AttachToActor(UGameplayStatics::GetActorOfClass(GetWorld(), AttachActorClass), Rules);
	}
}

void AKeyPad::Interact()
{
	APhase* Player = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(Controller);
	check(Player);
	
	if (bCanEnter)
	{
		Controller->SetViewTargetWithBlend(TargetCamera->GetOwner(), CameraBlendTime);
		Player->GetCharacterMovement()->DisableMovement();
		bCanEnter = false;
	}
	else
	{
		Controller->SetViewTargetWithBlend(Player->Camera->GetOwner(), CameraBlendTime);
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		bCanEnter = true;
	}
	Initiate();

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
	EnterPassword->SetText(InitialCode);

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

	for (auto Button : ButtonArray)
	{
		AKeyActor* KeyActor = Cast<AKeyActor>(Button->GetChildActor());
		KeyActor->SetKeyPad(this);
	}
}

void AKeyPad::ButtonEnter(AKeyPad* KeyPad, FText Value, bool bConfirmed, bool IsBackSpace)
{

	if (bConfirmed)
	{
		OnConfirmed();
	}
	else
	{
		if (IsBackSpace)
		{
			PressBackspace();
		}
		else
		{
			if (ButtonClickSound) UGameplayStatics::PlaySound2D(this, ButtonClickSound);
			FString EnterPasswordString = EnterPassword->Text.ToString();
			if (EnterPasswordString.Len() < Password.ToString().Len() || EnterPasswordString.IsEmpty()) EnterPasswordString.Append(Value.ToString());
			EnterPassword->SetText(FText::FromString(EnterPasswordString));
		}
	}

}

void AKeyPad::PressBackspace()
{
	FString EnterPasswordString = EnterPassword->Text.ToString();
	FString ErasedPassword = UKismetStringLibrary::GetSubstring(EnterPasswordString, 0, EnterPasswordString.Len() - 1);
	EnterPassword->SetText(FText::FromString(ErasedPassword));
}

void AKeyPad::OnConfirmed()
{
	AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (EnterPassword->Text.ToString() == Password.ToString() && Controller)
	{
		if (CorrectSound) UGameplayStatics::PlaySound2D(this, CorrectSound);
		Mesh->SetMaterial(1, CorrectMaterial);
		Controller->SetShowMouseCursor(false);
		Controller->bEnableMouseOverEvents = false;
		Controller->bEnableClickEvents = false;

		for (auto Button : ButtonArray)
		{
			AKeyActor* KeyActor = Cast<AKeyActor>(Button->GetChildActor());
			KeyActor->Reset();
		}

		APhase* Player = Cast<APhase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		check(Player);
		
		//GetWorld()->GetTimerManager().SetTimer(CameraHandle, [this,Controller,Phase]
		//	{
		//		Controller->SetViewTargetWithBlend(Phase->Camera->GetOwner(), CameraBlendTime);
		//	},
		//	1.f,
		//	false
		//);

		if (InteractActor)
		{
			GetWorld()->GetTimerManager().SetTimer(InteractHandle, [this,Controller] 
				{
					AInteract* InteractItem = Cast<AInteract>(UGameplayStatics::GetActorOfClass(this, InteractActor));
					TScriptInterface<IInteractInterface> Interface = TScriptInterface<IInteractInterface>(InteractItem);
					if (InteractItem && Interface)
					{
						Controller->SetViewTarget(InteractItem->TargetCamera->GetOwner());
						Interface->Interact();
					}
				},
				InteractInterval
				,false
			);

		}

		EnterPassword->SetText(FText::FromString(FString("")));

		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	}
	else
	{
		if (CorrectSound) UGameplayStatics::PlaySound2D(this, WrongSound);

		for (auto Button : ButtonArray)
		{
			AKeyActor* KeyActor = Cast<AKeyActor>(Button->GetChildActor());
			KeyActor->Reset();
		}

		EnterPassword->SetText(FText::FromString(FString("")));
	}


}

void AKeyPad::SetViewPlayerCamera()
{
	AHMController* Controller = Cast<AHMController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APhase* Phase = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Controller);
	check(Phase);

	Controller->SetViewTarget(Phase->Camera->GetOwner());
}
