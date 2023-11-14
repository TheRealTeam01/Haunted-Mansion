#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarrior.h"
#include "Components/TimelineComponent.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "HauntedMension/Attribute/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarriorAIController.h"

ASkeletonWarrior::ASkeletonWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing Component");
	PawnSensingComponent->bSeePawns = false;
	PawnSensingComponent->bHearNoises = true;
	PawnSensingComponent->HearingThreshold = 300.f;

	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("Behavior Tree");

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>("DissolveTimeline");

	Attribute = CreateDefaultSubobject<UAttributeComponent>("Attributes");

	
}

void ASkeletonWarrior::BeginPlay()
{
	Super::BeginPlay();

	DissolveTimelineUpdate.BindUFunction(this, FName("UpdateDissolve"));

	if (DissolveMateialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMateialInstance, this);

		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName("Dissolve"), -0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName("Glow"), 17000.f);
	}

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &ASkeletonWarrior::Chase);
	}
}

void ASkeletonWarrior::GetHit_Implementation(const FVector& ImpactPoint)
{
}

void ASkeletonWarrior::Chase(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	ASkeletonWarriorAIController* AIController = Cast<ASkeletonWarriorAIController>(GetController());

	if(PawnInstigator != this && AIController)
	{
		AIController->SetSensedTarget(PawnInstigator);
	}
}

void ASkeletonWarrior::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DieMontage) 
	{
		AnimInstance->Montage_Play(DieMontage);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FOnMontageEnded MontageEnded;
	MontageEnded.BindWeakLambda(this, [this] (UAnimMontage* AnimMontage, bool bInterrupted)
		{
			if (bInterrupted)
			{

			}
			else Destroy();
		}
	);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, DieMontage);
}

void ASkeletonWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkeletonWarrior::StartDissolve()
{
	DissolveTimelineUpdate.BindDynamic(this, &ASkeletonWarrior::UpdateDissolve);

	if (DissolveCurve)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTimelineUpdate);
		DissolveTimeline->PlayFromStart();
		DissolveTimelineFinished.BindUFunction(this, FName("Die"));

		GetWorld()->GetTimerManager().SetTimer(ScreamHandle, [this] 
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance && ScreamMontage)
				{
					AnimInstance->Montage_Play(ScreamMontage);
				}
			},
		2.f, false);
	}
}

void ASkeletonWarrior::StopDissolve()
{
	if (DissolveTimeline->IsPlaying())
	{
		DissolveTimeline->Reverse();
	}
}

void ASkeletonWarrior::UpdateDissolve(float DeltaTime)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName("Dissolve"), DeltaTime);
	}
}

