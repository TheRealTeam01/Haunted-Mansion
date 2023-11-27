#include "HauntedMension/Interact/KeyActor.h"
#include "Components/TextRenderComponent.h"
#include "HauntedMension/Ai/Sevarog.h"
AKeyActor::AKeyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Key = CreateDefaultSubobject<UStaticMeshComponent>("Key");
	KeyCode = CreateDefaultSubobject<UTextRenderComponent>("KeyCode");
	KeyCode->SetupAttachment(Key);
}

void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	//
	//Key->OnClicked.AddDynamic(this, &AKeyActor::ButtonClicked);
}

void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AKeyActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	KeyCode->SetText(Number);
}

void AKeyActor::ButtonClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{

}

