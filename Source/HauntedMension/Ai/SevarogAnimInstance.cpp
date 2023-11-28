// Fill out your copyright notice in the Description page of Project Settings.


#include "SevarogAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Sevarog.h"

// BP ������ �����´�
USevarogAnimInstance::USevarogAnimInstance()
{
	// �ִϸ��̼� ��Ÿ�ָ� �����´�.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Script/Engine.AnimMontage'/Game/HauntedMansion/Animation/Sevarog_SkeletonMontage.Sevarog_SkeletonMontage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HM(TEXT("/Script/Engine.AnimMontage'/Game/HauntedMansion/Animation/SevarogAnimation/AM_SevarogDeath.AM_SevarogDeath'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("/Script/Engine.AnimMontage'/Game/HauntedMansion/Animation/Sevarog_SkeletonMontage.Sevarog_SkeletonMontage'"));

	if (HM.Succeeded()) {
		HitMontage = HM.Object;
	}
	if (DM.Succeeded()) {
		DeathMontage = DM.Object;
	}
}

void USevarogAnimInstance::NativeInitializeAnimation()
{
}

void USevarogAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if(IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		auto Character = Cast<ASevarog>(Pawn);
		if (Character) {
			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
		}
	}
}


// ���Ӱ����� ���ؼ� ���� ��Ÿ�ָ� �����´�
void USevarogAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	//Montage_JumpToSection(Name, AttackMontage);
}

FName USevarogAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void USevarogAnimInstance::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}
