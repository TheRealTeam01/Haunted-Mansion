
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

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>("DoorTimeline");
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	TimelineUpdate.BindDynamic(this, &ADoor::InteractDoor); //DoorTimelineUpdate에 BindFunction

	if (CurveFloat)
	{
		DoorTimeline->AddInterpFloat(CurveFloat, TimelineUpdate); // DoorCurveFloat동안 DoorTimelineUpdate에 Bind된 Function을 실행.
		TimelineFinish.BindUFunction(this, FName("ChangeState")); //Timeline이 끝나면 호출되는 함수 바인드
		DoorTimeline->SetTimelineFinishedFunc(TimelineFinish);
	}
}


void ADoor::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

}

void ADoor::ChangeState() // 문이 다 열리고나서 호출됨.
{
	IsOpening = !IsOpening;
	OnInteractEnded.Broadcast();
}

void ADoor::InteractDoor(float DeltaTime)
{

	if (TimelineUpdate.IsBound()) UE_LOG(LogTemp, Warning, TEXT("InteractDoor"));

	FRotator Rotation = FRotator(RotationPitch * DeltaTime, RotationYaw * DeltaTime, RotationRoll * DeltaTime);

	Door->SetRelativeRotation(Rotation);

}

void ADoor::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Character && Character->GetKeyState() == EKeyState::EKS_UnEquippedKey) // 열쇠 없이 열려고 할 때
	{
		if (!SetOpen)
		{
			AHMController* Controller = Cast<AHMController>(Character->Controller);
			if (Controller)
			{
				AHMHUD* HMHUD = Cast<AHMHUD>(Controller->GetHUD());
				if (HMHUD)
				{
					HMHUD->HMOverlay->BlinkText->SetText(FText::FromString(DoorText));
					HMHUD->HMOverlay->PlayBlink();
				}
			}
		}
		else // 문 열고서 열쇠를 소비했을 때 (문은 여닫을 수 있음)
		{
			if (!IsOpening) // 문이 다 열리기 전에는 Interact하지 못함.
			{
				IsOpening = true;

				if (IsOpened)
				{
					DoorTimeline->Reverse();
					if (DoorSound)
					{
						UGameplayStatics::SpawnSoundAtLocation(this, DoorSound, GetActorLocation(), GetActorRotation());
					}
				}
				else
				{
					DoorTimeline->PlayFromStart();
					if (DoorSound)
					{
						UGameplayStatics::SpawnSoundAtLocation(this, DoorSound, GetActorLocation(), GetActorRotation());
					}
				}

				IsOpened = !IsOpened;
			}
			

		}


	}
	else
	{
		if (TimelineUpdate.IsBound()) UE_LOG(LogTemp, Warning, TEXT("Bound"));

		if (!IsOpening && (!IsOpened || SetOpen && Character->GetKeyState() == EKeyState::EKS_EquippedKey)) // 처음 열쇠로 열 때
		{
			IsOpening = true;


			DoorTimeline->PlayFromStart();

			SetOpen = true;

			Character->SetKeyState(EKeyState::EKS_UnEquippedKey);

			if (DoorSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(this, DoorSound, GetActorLocation(), GetActorRotation());
			}

			IsOpened = !IsOpened;
		}


	}

}
