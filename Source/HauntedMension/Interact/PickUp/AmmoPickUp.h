#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interact/Interact.h"
#include "AmmoPickUp.generated.h"

class UImage;

UCLASS()
class HAUNTEDMENSION_API AAmmoPickUp : public AInteract
{
	GENERATED_BODY()
	
public:	
	AAmmoPickUp();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UTexture2D* AmmoImage;

};
