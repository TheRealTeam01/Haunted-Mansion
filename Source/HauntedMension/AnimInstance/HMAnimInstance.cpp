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
			FTransform RightHandTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World); // hand_r의 월드 위치
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - Character->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.f);
			// hand_r이 HitTarget 방향을 보도록 함.(기본 캐릭터의 bone의 X축 방향이 반대라서 hand_r에서 HitTarget을 빼준 값을 더함)

			FTransform MuzzleTipTransform = DefaultWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World); // MuzzleFlash의 월드 위치
			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X)); // MuzzleFlash소켓의 X축 방향
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 10000.f, FColor::Purple);
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), Character->GetHitTarget(), FColor::Black);
		}
	}



	

}
