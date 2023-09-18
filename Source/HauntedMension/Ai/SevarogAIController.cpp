// Fill out your copyright notice in the Description page of Project Settings.


#include "SevarogAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ASevarogAIController::ASevarogAIController() 
{
	// Blackboard를 가져온다
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT(""));
	if (BT.Succeeded()) {
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT(""));
	if (BD.Succeeded()) {
		BlackboardData = BD.Object;
	}
}

void ASevarogAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASevarogAIController::OnUnPossess()
{
	// 일정 시간이 되면 임의의 좌표로 이동하는 수준의 함수이다
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASevarogAIController::RandomMove()
{

}
