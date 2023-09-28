
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "Interact.generated.h"

UCLASS()
class HAUNTEDMENSION_API AInteract : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AInteract();

	virtual void Tick(float DeltaTime) override;

	void ShowInteractWidget(bool bShowWidget);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* InteractSphere;

	UFUNCTION()
		virtual void  OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void  OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InteractWidget;
private:



};
