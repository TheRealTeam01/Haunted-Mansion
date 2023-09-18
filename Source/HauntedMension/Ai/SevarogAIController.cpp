// Fill out your copyright notice in the Description page of Project Settings.


#include "SevarogAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ASevarogAIController::ASevarogAIController() 
{
	// Blackboard�� �����´�
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
	// ���� �ð��� �Ǹ� ������ ��ǥ�� �̵��ϴ� ������ �Լ��̴�
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ASevarogAIController::RandomMove()
{

}
