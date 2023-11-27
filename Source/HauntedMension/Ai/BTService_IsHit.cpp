// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/Ai/BTService_IsHit.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsHit::UBTService_IsHit()
{
	NodeName = TEXT("IsHitCheck");
}

void UBTService_IsHit::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
}
