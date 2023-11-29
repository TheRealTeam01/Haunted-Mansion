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
	auto TargetPawn = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();	// �� ��ü�� ������ �����´�
	FVector Center = CurrentPawn->GetActorLocation();	// �� ��ġ�� �����´�.
	float SearchRadius = 1500.0f;

	if (World == nullptr)
		return;
	// ����Ƽ�� overlap Sphere�� ����ϴ� ����, ���ڰ� ������ ���Ƽ� ������.
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// �츮�� �߰��� ������ 2ä���̹Ƿ� 2�� ���
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams
	);

	// ������ ã�� ���
	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			APhase* MyCharacter = Cast<APhase>(OverlapResult.GetActor());
			// ������ ���Ͷ� �÷��̾��� ��Ʈ�ѷ��� ���� ������, �÷��̾� ���θ� �ѹ� �� üũ�Ѵ�.
			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), MyCharacter);
				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
		//OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}
	// ������ ã�� ���Ѱ��
	else
	{
		// AI�� ���õ� ��� ������ �����ͼ� Target�� nullptr�� �о������.
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
}