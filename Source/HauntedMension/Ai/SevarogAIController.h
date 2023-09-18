// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SevarogAIController.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ASevarogAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASevarogAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void RandomMove();
	
private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
