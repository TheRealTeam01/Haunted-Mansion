#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interact/Interact.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "FlashLight.generated.h"

class UImage;

UCLASS()
class HAUNTEDMENSION_API AFlashLight : public AInteract
{
	GENERATED_BODY()
	
public:	
	AFlashLight();

	virtual void Tick(float DeltaTime) override;

	virtual void  OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void  OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void TriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void TriggerBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void Equip(USceneComponent* Inparent, AActor* NewOwner, APawn* NewInstigator);

	void LightOnOff(bool LightOnOff);

	virtual void Interact() override;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* TriggerBox;

	bool bLightOn = false;

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	class USpotLightComponent* Light;

	UPROPERTY(EditAnywhere)
		UTexture2D* FlashLightImage;

	float InitialBrightness = 0.f;

	float OnBrightness = 15000.f;

public:

	FORCEINLINE bool GetbLightOn() { return bLightOn; }
};
