// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/AnimInstance/HMAnimInstance.h"
#include "HauntedMension/Character/Phase.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"
#include "HauntedMension/Weapon/Weapon.h"


void UHMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<APhase>(TryGetPawnOwner());

	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void UHMAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement && Character)
	{
		Speed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);

		IsFalling = CharacterMovement->IsFalling();

		IsAccelerating = CharacterMovement->GetCurrentAcceleration().Size() > 0.f ? true : false;

		bRotateRootBone = Character->GetbRotateRootBone();

		TurningInPlace = Character->GetTurningInPlace();

		DefaultWeapon = Character->GetDefaultWeapon();

		AO_Yaw = Character->GetAO_Yaw();
		AO_Pitch = Character->GetAO_Pitch();

		if (DefaultWeapon && DefaultWeapon->GetWeaponMesh() && Character->GetMesh()) 
		{
			FTransform RightHandTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World); // hand_r�� ���� ��ġ
			RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - Character->GetHitTarget())); 
			// hand_r�� HitTarget ������ ������ ��.(�⺻ ĳ������ bone�� X�� ������ �ݴ�� hand_r���� HitTarget�� ���� ���� ����)

			FTransform MuzzleTipTransform = DefaultWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World); // MuzzleFlash�� ���� ��ġ
			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X)); // MuzzleFlash������ X�� ����
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 10000.f, FColor::Purple);
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), Character->GetHitTarget(), FColor::Black);
		}
	}



	

}
