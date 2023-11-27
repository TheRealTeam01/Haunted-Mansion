#include "HauntedMension/Interact/Drawer.h"
#include "Components/TimeLineComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ADrawer::ADrawer()
{
	PrimaryActorTick.bCanEverTick = true;

	Handle1 = CreateDefaultSubobject<UStaticMeshComponent>("Handle1_1");
	Handle1->SetupAttachment(Mesh);
	
	Handle1_2 = CreateDefaultSubobject<UStaticMeshComponent>("Handle1_2");
	Handle1_2->SetupAttachment(Mesh);

	DrawerTimeline = CreateDefaultSubobject<UTimelineComponent>("DrawTimeline");

}

void ADrawer::BeginPlay()
{
	Super::BeginPlay();
	
	if (DrawerCurve)
	{
		DrawerTimelineUpdate.BindDynamic(this, &ADrawer::Move);

		DrawerTimeline->AddInterpFloat(DrawerCurve, DrawerTimelineUpdate);
	}
}

void ADrawer::Interact()
{
	if (!IsOpened)
	{
		DrawerTimeline->PlayFromStart();
		IsOpened = true;
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UGameplayStatics::PlaySoundAtLocation(this, DrawerSound, GetActorLocation());
}

void ADrawer::Move(float DeltaTime)
{
	FVector Location(GetActorLocation().X, GetActorLocation().Y + MoveValue * DeltaTime, GetActorLocation().Z);

	SetActorRelativeLocation(Location);
}

void ADrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

