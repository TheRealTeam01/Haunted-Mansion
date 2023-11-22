// Fill out your copyright notice in the Description page of Project Settings.


#include "SevarogAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ASevarogAIController::ASevarogAIController() 
{
	// Blackboard�� �����´�
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/HauntedMansion/Ai/BT_Sevarog.BT_Sevarog"));
	if (BT.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("BB Binding"));
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Script/AIModule.BlackboardData'/Game/HauntedMansion/Ai/BB_Sevarog.BB_Sevarog"));
	if (BD.Succeeded()) {
		BlackboardData = BD.Object;
	}
}

void ASevarogAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BlackboardData, BlackboardComp))
	{
		if (RunBehaviorTree(BehaviorTree)) 
		{
			// 성공시 BT를 사용하라
		}
	}
}

void ASevarogAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASevarogAIController::RandomMove()
{
	auto CurrentPawn = GetPawn();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, RandomLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
	}
}
