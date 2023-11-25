
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarriorAIController.h"
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarrior.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ASkeletonWarriorAIController::ASkeletonWarriorAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	check(Blackboard);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
	check(BehaviorTreeComponent);
}

void ASkeletonWarriorAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetBlackboardComponent()->SetValueAsBool(FName("IsStanding"), false);
}

