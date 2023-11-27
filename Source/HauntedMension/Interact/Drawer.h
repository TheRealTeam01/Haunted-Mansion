#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "Components/TimeLineComponent.h"
#include "Drawer.generated.h"

class USphereComponent;
class USoundBase;

UCLASS()
class HAUNTEDMENSION_API ADrawer : public AInteract
{
	GENERATED_BODY()
	
public:

	ADrawer();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void Interact();

	UFUNCTION()
	void Move(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DrawerTimeline;
	
	FOnTimelineFloat DrawerTimelineUpdate;

	FOnTimelineEvent DrawerTimelineFinished;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DrawerCurve;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Handle1;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Handle1_2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DrawerSound;

	UPROPERTY(EditAnywhere)
	float MoveValue = 1.f;

	UPROPERTY(EditAnywhere)
	bool IsOpened = false;

protected:



};
