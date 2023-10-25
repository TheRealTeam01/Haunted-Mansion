#include "StoneStatue.h"
#include "Kismet/GameplayStatics.h"

void AStoneStatue::Interact()
{
	if (!IsMove)
	{
		Timeline.PlayFromStart();
		
		IsMove = true;
		
		UE_LOG(LogTemp, Warning, TEXT("StoneStatue"));
	}
}

void AStoneStatue::StatueMove(float DeltaTime)
{
	FVector Location(GetActorLocation().X, GetActorLocation().Y + MoveValue * DeltaTime, GetActorLocation().Z);

	if (MoveSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, MoveSound, GetActorLocation(), GetActorRotation());
	}

	SetActorRelativeLocation(Location);

}

void AStoneStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void AStoneStatue::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		TimelineUpdate.BindDynamic(this, &AStoneStatue::StatueMove);
		Timeline.AddInterpFloat(CurveFloat, TimelineUpdate);
	}
}
