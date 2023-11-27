// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_IsHit.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API UBTService_IsHit : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_IsHit();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	void SetHitInfo();

private:
	bool bIsHit = false;
};
