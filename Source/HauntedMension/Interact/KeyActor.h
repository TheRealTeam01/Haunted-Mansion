#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyActor.generated.h"

class UTextRenderComponent;

UCLASS()
class HAUNTEDMENSION_API AKeyActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AKeyActor();

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void ButtonClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Key;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextRenderComponent> KeyCode;

	UPROPERTY(EditAnywhere)
		FText Number;
};
