#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"

class AInteract;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAUNTEDMENSION_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTraceComponent();

	UPROPERTY(EditAnywhere)
		float TraceLength = 10000.f;

	UPROPERTY(EditAnywhere)
		bool bShowDebug = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AInteract> PrevInteractItem;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
