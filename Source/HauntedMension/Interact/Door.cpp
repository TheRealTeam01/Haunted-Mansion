
#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimeLineComponent.h"
#include "Curves/CurveFloat.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Controller/HMController.h"
#include "HauntedMension/HUD/HMHUD.h"
#include "HauntedMension/HUD/HMOverlay.h"
#include "Components/TextBlock.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	DoorFrame->SetupAttachment(Mesh);
	DoorFrame->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorFrame->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	Door = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	Door->SetupAttachment(DoorFrame);
	Door->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Door->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (DoorCurveFloat)
	{
		DoorTimelineUpdate.BindDynamic(this, &ADoor::InteractDoor); //DoorTimelineUpdate�� BindFunction
		DoorTimeline.AddInterpFloat(DoorCurveFloat, DoorTimelineUpdate); // DoorCurveFloat���� DoorTimelineUpdate�� Bind�� Function�� ����.
	}
}


void ADoor::Tick(float DeltaTime)
{
	// Timeline�� Deltatime�� ����.
	DoorTimeline.TickTimeline(DeltaTime); //�̰� ���ؼ� ���� 1�ð� �Һ���..
}

void ADoor::InteractDoor(float DeltaTime)
{

	if (DoorTimelineUpdate.IsBound()) UE_LOG(LogTemp, Warning, TEXT("InteractDoor"));

	FRotator Rotation = FRotator(0.f, DoorRotateValue * DeltaTime, 0.f);

	Door->SetRelativeRotation(Rotation);

	if (DoorSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, DoorSound, GetActorLocation(), GetActorRotation());
	}
}

void ADoor::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Character && Character->GetKeyState() == EKeyState::EKS_UnEquippedKey)
	{
		AHMController* Controller = Cast<AHMController>(Character->Controller);
		if (Controller)
		{
			AHMHUD* HMHUD = Cast<AHMHUD>(Controller->GetHUD());
			if (HMHUD)
			{
				HMHUD->HMOverlay->BlinkText->SetText(FText::FromString("Don't Have Key. Find The Key"));
				HMHUD->HMOverlay->PlayBlink();
			}
		}
		
	}
	else
	{
		if (DoorTimelineUpdate.IsBound()) UE_LOG(LogTemp, Warning, TEXT("Bound"));

		if (!IsOpened) DoorTimeline.PlayFromStart();
		else DoorTimeline.Reverse();

		IsOpened = !IsOpened;
	}

}
