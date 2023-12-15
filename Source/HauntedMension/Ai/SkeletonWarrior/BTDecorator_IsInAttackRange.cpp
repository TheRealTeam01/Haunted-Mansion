
#include "HauntedMension/Ai/SkeletonWarrior/BTDecorator_IsInAttackRange.h"
#include "AIController.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"


bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	check(OwnerPawn);

	APhase* Target = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Target);

	return OwnerPawn->GetDistanceTo(Target) <= 300.f ? true : false;
}
