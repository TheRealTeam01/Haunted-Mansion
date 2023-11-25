// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "UBTService_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API UUBTService_IsInRange : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetPlayerSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DIstanceToTargetSelector;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//FBlackboardKeySelector IsStandingSelector;

};
