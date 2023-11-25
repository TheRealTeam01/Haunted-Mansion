
#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "HintPage.generated.h"

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
	TSubclassOf<UUserWidget> HintPageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PageNumber = 1;
};
