#include "HauntedMension/Ai/SkeletonWarrior/AttackAnimNotifyState.h"
#include "AttackAnimNotifyState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkeletonWarrior.h"
#include "Components/SphereComponent.h"

void UAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	FHitResult HitResult;

	if (MeshComp && MeshComp->GetOwner())
	{
		ASkeletonWarrior* SkeletonWarrior = Cast<ASkeletonWarrior>(MeshComp->GetOwner());
		if(SkeletonWarrior) SkeletonWarrior->AttackTrace();
	}
}

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ASkeletonWarrior* SkeletonWarrior = Cast<ASkeletonWarrior>(MeshComp->GetOwner());
		if(SkeletonWarrior) SkeletonWarrior->ActorsToIgnore.Empty();
	}
}
