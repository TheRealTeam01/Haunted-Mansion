#include "HauntedMension/Interact/TriggerActor.h"
#include "Components/BoxComponent.h"

ATriggerActor::ATriggerActor()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger Box");
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetupAttachment(GetRootComponent());

}

void ATriggerActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::OnBeginOverlap);

}

void ATriggerActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
