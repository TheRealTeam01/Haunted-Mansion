
#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "DoorKey.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ADoorKey : public AInteract
{
	GENERATED_BODY()
	
public:

	ADoorKey();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void Interact() override;

private:

	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> FallSound;
};
