#include "HauntedMension/Interact/KeyActor.h"
#include "Components/TextRenderComponent.h"
#include "HauntedMension/Ai/Sevarog.h"
#include "KeyPad.h"

AKeyActor::AKeyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Key = CreateDefaultSubobject<UStaticMeshComponent>("Key");
	//Key->DispatchOnClicked(EKeys::LeftMouseButton);
	KeyCode = CreateDefaultSubobject<UTextRenderComponent>("KeyCode");
	KeyCode->SetupAttachment(Key);
}

void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	
	Key->OnBeginCursorOver.AddDynamic(this, &AKeyActor::OnCursor);
	Key->OnEndCursorOver.AddDynamic(this, &AKeyActor::EndCursor);
	Key->OnClicked.AddDynamic(this, &AKeyActor::ButtonClicked);
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
	check(KeyPad);

	KeyPad->ButtonEnter(KeyPad, Number, bConfirmed, IsBackSpace);
}

void AKeyActor::OnCursor(UPrimitiveComponent* TouchedComponent)
{
	Key->SetMaterial(0,BeginHoverMaterial);
}

void AKeyActor::EndCursor(UPrimitiveComponent* TouchedComponent)
{
	Key->SetMaterial(0, EndHoverMaterial);
}

void AKeyActor::ResetButton()
{

}

