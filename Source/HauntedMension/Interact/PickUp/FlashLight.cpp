#include "FlashLight.h"
#include "Components/SpotLightComponent.h"
#include "Components/SphereComponent.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"

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
	
	/*Mesh->SetCustomDepthStencilValue(252);
	Mesh->MarkRenderStateDirty();
	EnableCustomDepth(true);*/
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

void AFlashLight::Interact()
{
	APhase* Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Character)
	{
		Character->PlayPickUpMontage();
		Character->SetFlashLightState(EFlashLightState::EFS_EquippedFlashLight);
		Character->SetEquippedFlashLightState(this);
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

	//EnableCustomDepth(false);
}

void AFlashLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

