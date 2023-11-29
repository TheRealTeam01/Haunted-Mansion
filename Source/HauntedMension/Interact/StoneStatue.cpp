#include "StoneStatue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SplineComponent.h"

AStoneStatue::AStoneStatue()
{
	MoveEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("Move Effect");
	MoveEffectComponent->SetupAttachment(Mesh);

	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void AStoneStatue::Interact()
{
	if (MoveSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, MoveSound, GetActorLocation(), GetActorRotation());
	}

	SpawnMoveEffect();

	PlayCameraShake();

	Timeline->PlayFromStart();
}

void AStoneStatue::StatueMove(float DeltaTime)
{
	FVector Location(GetActorLocation().X + LocationX * DeltaTime, GetActorLocation().Y + LocationY * DeltaTime, GetActorLocation().Z + LocationZ * DeltaTime);

	SetActorRelativeLocation(Location);

}

void AStoneStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStoneStatue::PlayCameraShake()
{
	if (CameraShake)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake);
	}
}

void AStoneStatue::SpawnMoveEffect()
{
	if (MoveEffectComponent) MoveEffectComponent->ActivateSystem();
}

void AStoneStatue::DestroyMoveEffect()
{
	if(MoveEffectComponent) MoveEffectComponent->DestroyComponent();
}

void AStoneStatue::BeginPlay()
{
	Super::BeginPlay();

	MoveEffectComponent->DeactivateSystem();
	
	if (CurveFloat)
	{
		TimelineUpdate.BindDynamic(this, &AStoneStatue::StatueMove);
		Timeline->AddInterpFloat(CurveFloat, TimelineUpdate);
		TimelineFinish.BindDynamic(this, &AStoneStatue::DestroyMoveEffect);
		Timeline->SetTimelineFinishedFunc(TimelineFinish);
	}

	
	//FVector StatueLocation(GetActorLocation().X, GetActorLocation().Y, ZLocation);

	//MoveEffectComponent = UGameplayStatics::SpawnEmitterAttached(MoveEffect, Mesh, FName(), StatueLocation, GetActorRotation(), EAttachLocation::SnapToTarget);

}
