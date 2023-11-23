// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/Ai/BTTask_Attack.h"
#include "Sevarog.h"
#include "SevarogAIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto SevarogCharacter = Cast<ASevarog>(OwnerComp.GetAIOwner()->GetPawn());
	if (SevarogCharacter == nullptr)
		return EBTNodeResult::Failed;
	
	SevarogCharacter->Attack();
	bIsAttacking = true;

	SevarogCharacter->OnAttackEnd.AddLambda([this]() {
		bIsAttacking = false;
		});

	return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (bIsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}