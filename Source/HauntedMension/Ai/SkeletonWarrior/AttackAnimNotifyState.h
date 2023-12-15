#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "AttackAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API UAttackAnimNotifyState : public UAnimNotifyState, public IHitInterface
{
	GENERATED_BODY()
	
public:

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);

};
