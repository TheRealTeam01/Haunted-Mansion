
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "Interact.generated.h"

class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractEnded);

UCLASS()
class HAUNTEDMENSION_API AInteract : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AInteract();

	virtual void Tick(float DeltaTime) override;

	virtual void ShowInteractWidget(bool bShowWidget);

	void EnableCustomDepth(bool Enable);

	virtual void  OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void  OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* TargetCamera;

	FOnInteractEnded OnInteractEnded;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* InteractWidget;

	virtual void Interact() override;

public:

	FORCEINLINE UStaticMeshComponent* GetInteractMesh() { return Mesh; }
};
