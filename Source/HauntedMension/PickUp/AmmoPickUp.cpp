#include "AmmoPickUp.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Weapon/Weapon.h"

AAmmoPickUp::AAmmoPickUp()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmoPickUp::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AAmmoPickUp::OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


void AAmmoPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

