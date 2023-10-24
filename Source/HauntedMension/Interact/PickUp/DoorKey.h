
#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "DoorKey.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ADoorKey : public AInteract
{
	GENERATED_BODY()
	
public:

	ADoorKey();

	virtual void Interact() override;
};
