
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SkeletonWarriorAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ASkeletonWarriorAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ASkeletonWarriorAIController();

	virtual void OnPossess(APawn* InPawn) override;

	void SetSensedTarget(APawn* Target);

protected:

	UPROPERTY(VisibleAnywhere)
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly)
	FName TargetKey = "SensedPawn";

private:

};
