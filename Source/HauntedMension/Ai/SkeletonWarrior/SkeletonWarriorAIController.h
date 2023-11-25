
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SkeletonWarriorAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;
class UAISensing;

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

protected:

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;

private:

};
