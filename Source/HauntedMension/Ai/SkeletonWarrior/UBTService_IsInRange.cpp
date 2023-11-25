
#include "HauntedMension/Ai/SkeletonWarrior/UBTService_IsInRange.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "HauntedMension/AI/SkeletonWarrior/SkeletonWarrior.h"

void UUBTService_IsInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	check(OwningPawn);

	ASkeletonWarrior* SkeletonWarrior = Cast<ASkeletonWarrior>(OwningPawn);
	check(OwningPawn);

	APhase* Player = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Player);

	float DistanceToTarget = OwningPawn->GetDistanceTo(Player);

	//bool IsStanding = SkeletonWarrior->GetIsStanding();

	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("%s"),IsStanding));

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetPlayerSelector, Player);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DIstanceToTargetSelector, DistanceToTarget);
	//UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsStandingSelector, IsStanding);
}

