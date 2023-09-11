// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/AnimInstance/HMAnimInstance.h"
#include "HauntedMension/Character/Phase.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"


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

	if (CharacterMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);

		IsFalling = CharacterMovement->IsFalling();

		IsAccelerating = CharacterMovement->GetCurrentAcceleration().Size() > 0.f ? true : false;
	}
}
