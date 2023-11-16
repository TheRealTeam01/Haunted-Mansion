// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API UUBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UUBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	bool IsAttacking;
};
