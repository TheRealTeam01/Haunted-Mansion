
#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "Door.generated.h"

class UCurveFloat;
class USoundBase;
/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ADoor : public AInteract
{

	GENERATED_BODY()

public:

	ADoor();

	virtual void Interact() override;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InteractDoor(float DeltaTime);
private:

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> DoorFrame;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CurveFloat;

	FTimeline Timeline;

	FOnTimelineFloat TimelineUpdate;

	UPROPERTY(EditAnywhere)
	float DoorRotateValue = 120.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DoorSound;

	UPROPERTY(VisibleAnywhere)
		bool IsOpened = false;

	UPROPERTY(VisibleAnywhere)
		bool SetOpen = false;

};
