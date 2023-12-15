 #include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarrior.h"
#include "Components/TimelineComponent.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "HauntedMension/Attribute/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HauntedMension/Ai/SkeletonWarrior/SkeletonWarriorAIController.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ASkeletonWarrior::ASkeletonWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASkeletonWarriorAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	RightHandSphere = CreateDefaultSubobject<USphereComponent>("RightHand Sphere");
	RightHandSphere->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	RightHandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	RightHandSphere->SetGenerateOverlapEvents(true);

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>("DissolveTimeline");

	DieTimeline = CreateDefaultSubobject<UTimelineComponent>("DieTimeline");

	Attribute = CreateDefaultSubobject<UAttributeComponent>("Attributes");

}

void ASkeletonWarrior::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		IsAttacking = true;

		int32 RandValue = FMath::RandRange(1, 3);
		FName SectionName;

		switch (RandValue)
		{
			case 1:
				SectionName = FName("Attack1");
				break;
			case 2:
				SectionName = FName("Attack2");
				break;
			case 3:
				SectionName = FName("Attack3");
				break;
		}

		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		
		FOnMontageEnded MontageEnded;

		MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted)
			{
				if (bInterrupted)
				{
					IsAttacking = false;
				}
				else
				{
					IsAttacking = false;
					OnAttackEnded.Broadcast();
				}
			}
		);
	}
}

void ASkeletonWarrior::BeginPlay()
{
	Super::BeginPlay();

	DissolveTimelineUpdate.BindUFunction(this, FName("UpdateDissolve"));

	Tags.Add(FName("Skeleton"));
}

void ASkeletonWarrior::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (IsDying) return;

	DissolveTimeline->Deactivate();
	
	Die();

}

void ASkeletonWarrior::InitializeAIComponents()
{
	AIController = Cast<ASkeletonWarriorAIController>(Controller);

	AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
}

void ASkeletonWarrior::UpdateDie(float DeltaTime)
{
	if (DynamicDieMaterialInstance)
	{
		DynamicDieMaterialInstance->SetScalarParameterValue(FName("Dissolve"), DeltaTime);
	}
}

void ASkeletonWarrior::Die()
{

	if (DieMateialInstance)
	{
		DynamicDieMaterialInstance = UMaterialInstanceDynamic::Create(DieMateialInstance, this);

		GetMesh()->SetMaterial(0, DynamicDieMaterialInstance);
		DynamicDieMaterialInstance->SetScalarParameterValue(FName("Dissolve"), -0.55f);
		DynamicDieMaterialInstance->SetScalarParameterValue(FName("Glow"), 20000.f);
	}

	if (DieCurve)
	{
		DieTimelineUpdate.BindDynamic(this, &ASkeletonWarrior::UpdateDie);
		DieTimeline->AddInterpFloat(DieCurve, DieTimelineUpdate);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->DisableMovement();
	IsDying = true;

	DieTimeline->PlayFromStart();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DieMontage) 
	{
		AnimInstance->Montage_Play(DieMontage);
	}

	FOnMontageEnded MontageEnd;
	MontageEnd.BindWeakLambda(this, [this](UAnimMontage* Animmontage, bool bInterrupted)
		{
			if (bInterrupted) 
			{
				Destroy();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Destroy"));
				Destroy();
			}
		});
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DieMontage);
}

void ASkeletonWarrior::StandUp()
{
	if (IsStanding) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && StandUpMontage)
	{
		AnimInstance->Montage_Play(StandUpMontage);
	}

	FOnMontageEnded MontageEnded;
	MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted)
		{
			if (bInterrupted)
			{

			} 
			else
			{
				IsStanding = true;
			}
		}
	);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, StandUpMontage);
}

void ASkeletonWarrior::AttackTrace()
{
	FHitResult HitResult;

	const FVector Start = GetMesh()->GetSocketLocation(FName("RightHandSocket_Start"));
	const FVector End = GetMesh()->GetSocketLocation(FName("RightHandSocket_End"));

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		TraceRadius,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		ShowDebugBox ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		if (HitResult.GetActor()->ActorHasTag(FName("Skeleton"))) return;

		TScriptInterface<IHitInterface> Interface = TScriptInterface<IHitInterface>(HitResult.GetActor());
		if (Interface)
		{
			Interface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint);
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, Controller, this, UDamageType::StaticClass());
			ActorsToIgnore.Add(HitResult.GetActor());
		}
	}
}

FVector ASkeletonWarrior::GetTargetRotation()
{
	APhase* Target = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Target);

	return Target->GetActorLocation();
}

FVector ASkeletonWarrior::GetTargetLocation()
{
	APhase* Target = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	const FVector CombatTargetLocation = Target->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpDistance;
	return CombatTargetLocation + TargetToMe;
}

void ASkeletonWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkeletonWarrior::StartDissolve()
{
	if (IsDissolving || IsDying) return;

	if (DissolveMateialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMateialInstance, this);

		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName("Dissolve"), -0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName("Glow"), 20000.f);
	}

	DissolveTimelineUpdate.BindDynamic(this, &ASkeletonWarrior::UpdateDissolve);

	if (DissolveCurve)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTimelineUpdate);
		DissolveTimeline->PlayFromStart();
		IsDissolving = true;
		DissolveTimelineFinished.BindDynamic(this, &ASkeletonWarrior::DissolveDie);
		DissolveTimeline->SetTimelineFinishedFunc(DissolveTimelineFinished);
	}
}

void ASkeletonWarrior::DissolveDie()
{
	if (DissolveTimeline->IsPlaying() || !IsDissolving || IsDying) return;
	
	
	Destroy();

	UE_LOG(LogTemp, Warning, TEXT("Dissolve Die"));
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance && ScreamMontage)
	//{
	//	AnimInstance->Montage_Play(ScreamMontage);
	//}

	//FOnMontageEnded MontageEnded;
	//MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted)
	//	{
	//		if (bInterrupted)
	//		{
	//		}
	//		else Destroy();
	//	}
	//);
	//AnimInstance->Montage_SetEndDelegate(MontageEnded, ScreamMontage);
}

void ASkeletonWarrior::StopDissolve()
{
	if (DissolveTimeline->IsPlaying())
	{
		DissolveTimeline->Reverse();
		IsDissolving = false;
	}
}

void ASkeletonWarrior::UpdateDissolve(float DeltaTime)
{
	if (IsDying) return;

	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName("Dissolve"), DeltaTime);
	}
}

