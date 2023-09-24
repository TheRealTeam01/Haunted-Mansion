
#include "Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Interfaces/HitInterface.h"

ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;


	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	GeometryCollection->SetupAttachment(RootComponent);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); // 파편들이 카메라 Block 안되도록.
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); // 파편들이 Block 안되도록.

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GeometryCollection);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block); // Pawn이 통과하지 못하도록.

}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakable::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (FractionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FractionSound,
			ImpactPoint
		);
	}
}

void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

