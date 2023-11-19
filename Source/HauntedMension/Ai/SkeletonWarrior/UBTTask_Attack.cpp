#include "HauntedMension/Ai/SkeletonWarrior/UBTTask_Attack.h"
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarrior.h"
#include "AIController.h"
#include "Gameframework/CharacterMovementComponent.h"

UUBTTask_Attack::UUBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UUBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASkeletonWarrior* SkeletonWarrior = Cast<ASkeletonWarrior>(OwnerComp.GetAIOwner()->GetPawn());
	check(SkeletonWarrior);

	SkeletonWarrior->Attack();
	
	IsAttacking = true;

	SkeletonWarrior->OnAttackEnded.AddLambda([this,SkeletonWarrior]() 
		{
			IsAttacking = false;
		}
	);


	return EBTNodeResult::Succeeded;
}

void UUBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking) FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
