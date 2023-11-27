
#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "HintPage.generated.h"

class UCameraComponent;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AHintPage : public AInteract
{
	GENERATED_BODY()
	
public:

	AHintPage();

	virtual void Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReading = false;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UCameraComponent> InteractCamera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HintPageWidget;

	FTimerHandle CameraHandle;

	UPROPERTY(EditAnywhere)
		float CameraBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PageNumber = 1;
};
