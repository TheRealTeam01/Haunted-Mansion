
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "Interact.generated.h"

class USceneComponent;

UCLASS()
class HAUNTEDMENSION_API AInteract : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AInteract();

	virtual void Tick(float DeltaTime) override;

	void ShowInteractWidget(bool bShowWidget);

	void EnableCustomDepth(bool Enable);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* InteractSphere;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* InteractWidget;

	UFUNCTION()
		virtual void  OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void  OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Interact() override;

private:



};
