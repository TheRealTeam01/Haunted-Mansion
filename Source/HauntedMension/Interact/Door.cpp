
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
	
	TimelineUpdate.BindDynamic(this, &ADoor::InteractDoor); //DoorTimelineUpdate�� BindFunction

	if (CurveFloat)
	{
		DoorTimeline->AddInterpFloat(CurveFloat, TimelineUpdate); // DoorCurveFloat���� DoorTimelineUpdate�� Bind�� Function�� ����.
		TimelineFinish.BindUFunction(this, FName("ChangeState")); //Timeline�� ������ ȣ��Ǵ� �Լ� ���ε�
		DoorTimeline->SetTimelineFinishedFunc(TimelineFinish);
	}
}


void ADoor::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

}

void ADoor::ChangeState() // ���� �� �������� ȣ���.
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
	if (Character && Character->GetKeyState() == EKeyState::EKS_UnEquippedKey) // ���� ���� ������ �� ��
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
		else // �� ���� ���踦 �Һ����� �� (���� ������ �� ����)
		{
			if (!IsOpening) // ���� �� ������ ������ Interact���� ����.
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

		if (!IsOpening && (!IsOpened || SetOpen && Character->GetKeyState() == EKeyState::EKS_EquippedKey)) // ó�� ����� �� ��
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
