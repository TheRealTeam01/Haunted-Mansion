#include "Book.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Interact/StoneStatue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "HauntedMension/Controller/HMController.h"
#include "Camera/CameraComponent.h"
#include "HauntedMension/Weapon/Weapon.h"

ABook::ABook()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh->SetupAttachment(Root);

	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void ABook::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		TimelineUpdate.BindDynamic(this, &ABook::BookMove);
		Timeline->AddInterpFloat(CurveFloat, TimelineUpdate);
		TimelineFinish.BindUFunction(this, FName("SetPhysics"));
		Timeline->SetTimelineFinishedFunc(TimelineFinish);
	}

	if(IsSequenceUse) UGameplayStatics::GetActorOfClass(GetWorld(), SequencePhase)->SetActorHiddenInGame(true);

	/*Mesh->OnComponentHit.AddDynamic(this, &ABook::OnHit);*/
}

	
void ABook::StoneStatueInteract()
{
	TArray<AActor*> StoneStatues;

	UGameplayStatics::GetActorOfClass(GetWorld(), SequencePhase)->Destroy();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStoneStatue::StaticClass(), StoneStatues); //StoneStatue클래스인 엑터들을 가져와 StoneStatues배열에 담아서 반환.
	for (auto statue : StoneStatues)
	{
		TObjectPtr<AStoneStatue> StoneStatue = Cast<AStoneStatue>(statue);
		if (StoneStatue)
		{
			TScriptInterface<IInteractInterface> Interface = TScriptInterface<IInteractInterface>(StoneStatue);
			if (Interface)
			{
				StoneStatue->Interact();
			}
		}
	}
	
	Destroy();

}

void ABook::SetPhysics()
{
	Mesh->SetSimulatePhysics(true);
}

//void ABook::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	if (FallSound)
//	{	
//		UGameplayStatics::SpawnSoundAtLocation(this, FallSound, GetActorLocation(), GetActorRotation());
//	}
//}

void ABook::Interact()
{
	
	ShowInteractWidget(false);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UGameplayStatics::GetActorOfClass(GetWorld(), SequencePhase)->SetActorHiddenInGame(false);

	FMovieSceneSequencePlaybackSettings  PlayBackSettings;
	PlayBackSettings.bDisableLookAtInput = true;
	PlayBackSettings.bDisableMovementInput = true;
	PlayBackSettings.bHideHud = true;

	ALevelSequenceActor* LevelSequenceActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlayBackSettings, LevelSequenceActor);
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ABook::StoneStatueInteract);
	LevelSequencePlayer->Play();

	
}

void ABook::BookMove(float DeltaTime)
{
	FVector Location(GetActorLocation().X + LocationX * DeltaTime, GetActorLocation().Y + LocationY * DeltaTime, GetActorLocation().Z + LocationZ * DeltaTime);

	SetActorRelativeLocation(Location);
}

void ABook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


