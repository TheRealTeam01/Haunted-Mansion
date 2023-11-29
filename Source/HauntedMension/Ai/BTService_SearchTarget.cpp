// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/Ai/BTService_SearchTarget.h"
#include "HauntedMension/Ai/SevarogAIController.h"
#include "HauntedMension/Ai/Sevarog.h"
#include "Kismet/GamePlayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");	
	Interval = 1.0f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// Ÿ���� �ڵ� �󿡼� �����Ѵ�.
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	APhase* Phase = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();	// �� ��ü�� ������ �����´�
	FVector Center = CurrentPawn->GetActorLocation();	// �� ��ġ�� �����´�.
	float SearchRadius = 1500.0f;

	if (World == nullptr)
		return;
	
	FVector Distance = Phase->GetActorLocation() - CurrentPawn->GetActorLocation();
	if(Distance.Size() < SearchRadius)
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Phase);
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
}