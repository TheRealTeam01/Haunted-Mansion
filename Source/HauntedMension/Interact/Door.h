
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

	TObjectPtr<UTimelineComponent> DoorTimeline;

	FOnTimelineFloat TimelineUpdate;

	FOnTimelineEvent TimelineFinish;

	UPROPERTY(EditAnywhere)
	FString DoorText;

	UPROPERTY(EditAnywhere)
		float RotationPitch = 0.f;
	
	UPROPERTY(EditAnywhere)
		float RotationYaw = 0.f;
	
	UPROPERTY(EditAnywhere)
		float RotationRoll = 0.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DoorSound;

	UPROPERTY(EditAnywhere) // ���� ���� �������� ���� �������� �˷��ִ� ����.
		bool IsOpened = false;

	UPROPERTY(EditAnywhere) // �ѹ� ���� ���� ä�� �α����� ����
		bool SetOpen = false;

	UPROPERTY(VisibleAnywhere) // ���� ������ ���� Interact���ϵ��� �ϴ� ����
		bool IsOpening = false;
};
