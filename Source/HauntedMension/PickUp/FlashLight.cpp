#include "FlashLight.h"
#include "Components/SpotLightComponent.h"
#include "Components/SphereComponent.h"
#include "HauntedMension/Character/Phase.h"

// Sets default values
AFlashLight::AFlashLight()
{
	PrimaryActorTick.bCanEverTick = true;

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh, FName("FlashSocket"));
	
}

void AFlashLight::BeginPlay()
{
	Super::BeginPlay();	

	Light->SetIntensity(InitialBrightness);
}

void AFlashLight::LightOnOff(bool LightOnOff)
{
	bLightOn = LightOnOff;

	if (bLightOn)
	{
		Light->SetIntensity(OnBrightness);
	}
	else
	{
		Light->SetIntensity(InitialBrightness);
	}
}

void AFlashLight::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AFlashLight::OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AFlashLight::Equip(USceneComponent* Inparent, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);

	SetInstigator(NewInstigator);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	Mesh->AttachToComponent(Inparent, AttachmentRules, FName("LeftHandSocket"));

	InteractSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AFlashLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

