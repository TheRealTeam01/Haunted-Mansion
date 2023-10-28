#include "HauntedMension/Interact/PickUp/DoorKey.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"

ADoorKey::ADoorKey()
{
	Mesh->SetSimulatePhysics(true);
}

void ADoorKey::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ADoorKey::OnHit);
}

void ADoorKey::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (FallSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, FallSound, GetActorLocation(), GetActorRotation());
	}
}

void ADoorKey::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->PlayPickUpMontage();
		Character->SetKeyState(EKeyState::EKS_EquippedKey);
		this->Destroy();
	}
}
