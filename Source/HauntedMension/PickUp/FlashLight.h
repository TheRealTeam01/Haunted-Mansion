#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interact/Interact.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "FlashLight.generated.h"

UCLASS()
class HAUNTEDMENSION_API AFlashLight : public AInteract
{
	GENERATED_BODY()
	
public:	
	AFlashLight();

	virtual void Tick(float DeltaTime) override;

	virtual void  OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void  OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	void Equip(USceneComponent* Inparent, AActor* NewOwner, APawn* NewInstigator);

	void LightOnOff(bool LightOnOff);

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	class USpotLightComponent* Light;

	bool bLightOn = false;
	
	float InitialBrightness = 0.f;

	float OnBrightness = 15000.f;

public:

	FORCEINLINE bool GetbLightOn() { return bLightOn; }
};
