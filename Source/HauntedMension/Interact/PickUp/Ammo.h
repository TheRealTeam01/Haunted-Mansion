#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AAmmo : public AInteract
{
	GENERATED_BODY()
	
public:

	AAmmo();

	virtual void Interact() override;

};
