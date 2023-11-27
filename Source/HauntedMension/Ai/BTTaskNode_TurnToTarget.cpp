// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/Ai/BTTaskNode_TurnToTarget.h"
#include "Sevarog.h"
#include "SevarogAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto SevarogCharacter = Cast<ASevarog>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == SevarogCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<ASevarog>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASevarogAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	FVector LockVector = Target->GetActorLocation() - SevarogCharacter->GetActorLocation();
	LockVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LockVector).Rotator();
	SevarogCharacter->SetActorRotation(FMath::RInterpTo(SevarogCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
