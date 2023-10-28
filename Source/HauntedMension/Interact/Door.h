
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
	void ChangeState();

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

	FOnTimelineEvent TimelineFinish;

	UPROPERTY(EditAnywhere)
	float DoorRotateValue = 120.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DoorSound;

	UPROPERTY(VisibleAnywhere) // 문이 열린 상태인지 닫힌 상태인지 알려주는 변수.
		bool IsOpened = false;

	UPROPERTY(VisibleAnywhere) // 한번 열면 열린 채로 두기위한 변수
		bool SetOpen = false;

	UPROPERTY(VisibleAnywhere) // 문이 열리는 동안 Interact못하도록 하는 변수
		bool IsOpening = false;
};
