
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarriorAIController.h"
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarrior.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ASkeletonWarriorAIController::ASkeletonWarriorAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
}

void ASkeletonWarriorAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASkeletonWarrior* Skeleton = Cast<ASkeletonWarrior>(InPawn);
	if (Skeleton && Skeleton->BehaviorTree->BlackboardAsset)
	{
		BlackboardComponent->InitializeBlackboard(*Skeleton->BehaviorTree->BlackboardAsset);

		BehaviorTreeComponent->StartTree(*Skeleton->BehaviorTree);
	}
}

void ASkeletonWarriorAIController::SetSensedTarget(APawn* Target)
{
	if (BlackboardComponent) BlackboardComponent->SetValueAsObject(TargetKey, Target);
}
