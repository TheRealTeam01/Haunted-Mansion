// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/Ai/BTService_IsHit.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "HauntedMension/Ai/Sevarog.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsHit::UBTService_IsHit()
{
	NodeName = TEXT("IsHitCheck");
}

void UBTService_IsHit::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APhase* Phase = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ASevarog* Sevarog = Cast<ASevarog>(OwnerComp.GetAIOwner()->GetPawn());
	if (Phase == nullptr)
		return;

	Sevarog->OnHitInfo.AddLambda([this]() -> void {
		SetHitInfo();
		});

	if (bIsHit)
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Phase);
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
}

void UBTService_IsHit::SetHitInfo()
{
	bIsHit = !bIsHit;
}
