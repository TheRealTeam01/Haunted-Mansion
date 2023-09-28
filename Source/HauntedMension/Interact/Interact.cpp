#include "Interact.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "HauntedMension/Character/Phase.h"
#include "Components/WidgetComponent.h"
#include "HauntedMension/Character/Phase.h"

AInteract::AInteract()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(Mesh);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetRenderCustomDepth(false);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Sphere"));
	InteractSphere->SetupAttachment(Mesh);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(Mesh);
	
}

void AInteract::BeginPlay()
{
	Super::BeginPlay();

	if (InteractSphere)
	{
		InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteract::OnSphereOverlap);

		InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AInteract::OnEndSphereOverlap);
	}

	if(InteractWidget) InteractWidget->SetVisibility(false);
}

void AInteract::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APhase* Character = Cast<APhase>(OtherActor);
	
	if (Character && InteractWidget)
	{
		Character->SetOverlappingInteractitem(this);
		ShowInteractWidget(true);
	}
}

void AInteract::OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APhase* Character = Cast<APhase>(OtherActor);

	if (Character && InteractWidget)
	{
		Character->SetOverlappingInteractitem(nullptr);
		ShowInteractWidget(false);
	}
}

void AInteract::EnableCustomDepth(bool Enable)
{
	Mesh->SetRenderCustomDepth(Enable);
}

void AInteract::ShowInteractWidget(bool bShowWidget)
{
	if (InteractWidget)
	{
		InteractWidget->SetVisibility(bShowWidget);
	}
}

void AInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

