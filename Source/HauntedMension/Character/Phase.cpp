#include "HauntedMension/Character/Phase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "InputActionValue.h"
#include "HauntedMension/PickUp/FlashLight.h"
#include "HauntedMension/Interact/Interact.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "HauntedMension/Weapon/Weapon.h"
#include "HauntedMension/HMTypes/HMTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


APhase::APhase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 150.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	CharacterMovement = GetCharacterMovement();
	CharacterMovement->MaxWalkSpeed = 150.f;

	TurnInPlace = ETurnInPlace::ETIP_NotTurning;

	bUseControllerRotationYaw = false;
}

void APhase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	FlashLightState = EFlashLightState::EFS_UnEquippedFlashLight;

	GetMesh()->HideBoneByName(FName("teddy_bear_root"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(FName("heart_box"), EPhysBodyOp::PBO_None);

	if (Camera)
	{
		DefaultFOV = Camera->FieldOfView;
		CurrentFOV = DefaultFOV;
	}

	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon"));

		DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(Weapon);
		if (DefaultWeapon)
		{
			FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
			DefaultWeapon->AttachToComponent(GetMesh(), AttachmentRule, FName("RightHandSocket"));
			DefaultWeapon->SetOwner(this);
		}
	}

}

void APhase::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector FowardDirecton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(FowardDirecton, MovementVector.X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.Y);

}

void APhase::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	
	if (GetController())
	{
		AddControllerYawInput(LookAxis.X);
		AddControllerPitchInput(LookAxis.Y);
	}

}

void APhase::Jump(const FInputActionValue& Value)
{
	ACharacter::Jump();
}

void APhase::RunPressed()
{
	CharacterMovement->MaxWalkSpeed = 600.f;
}

void APhase::RunReleased()
{
	CharacterMovement->MaxWalkSpeed = 150.f;
}

void APhase::HideMeshifCameraClose()
{
	float DistanceToCamera = (Camera->GetComponentLocation() - GetActorLocation()).Size();
	//UE_LOG(LogTemp, Warning, TEXT("Distance : %f"), DistanceToCamera);
	if (DistanceToCamera < CameraDistanceThresHold)
	{
		GetMesh()->SetVisibility(false);
	}
	else
	{
		GetMesh()->SetVisibility(true);
	}
}

void APhase::InteractPressed()
{
	if (FlashLight)
	{
		PlayPickUpMontage();
		EquippedFlashLight = FlashLight;
		PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController && CharacterMovement)
		{
			//FInputModeUIOnly UIOnlyMode;
			//PlayerController->SetInputMode(UIOnlyMode);
			CharacterMovement->MaxWalkSpeed = 0.f;
		}
	}
}

void APhase::AimPressed()
{
	if (DefaultWeapon == nullptr) return;

	bAiming = true;
	ActionState = EActionState::EAS_Aiming;
}

void APhase::AimReleased()
{
	if (DefaultWeapon == nullptr) return;

	 bAiming = false;
	 ActionState = EActionState::EAS_Unoccupied;

}

void APhase::InterpFOV(float DeltaTime)
{
	if (DefaultWeapon == nullptr) return;

	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, ZoomedFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	if (Camera)
	{
		Camera->SetFieldOfView(CurrentFOV);
	}
}

void APhase::FlashOnOffPressed()
{
	if (EquippedFlashLight)
	{
		if (!EquippedFlashLight->GetbLightOn())
		{
			EquippedFlashLight->LightOnOff(true);
		}
		else
		{
			EquippedFlashLight->LightOnOff(false);
		}
	}
}

void APhase::PlayPickUpMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PickupMontage)
	{
		AnimInstance->Montage_Play(PickupMontage);
	}
}

void APhase::Fire()
{
	if (ActionState == EActionState::EAS_Aiming)
	{
		DefaultWeapon->Fire(HitTarget);
	}
}

void APhase::TraceCrossHair(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrossHairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrossHairWorldPosition; // 월드 좌표
	FVector CrossHairWorldDirection; // 방향 


	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld( //주어진 2D 화면 공간 좌표를 3D 세계 공간 지점과 방향으로 변환.
		UGameplayStatics::GetPlayerController(this, 0) ,
		CrossHairLocation, 
		CrossHairWorldPosition,
		CrossHairWorldDirection
	);
	
	if (bScreenToWorld)
	{
		FVector TraceStart = CrossHairWorldPosition;

		float DistanceToCharacter = (GetActorLocation() - TraceStart).Size();
		TraceStart += CrossHairWorldDirection * (DistanceToCharacter * 100.f); // 캐릭터보다 앞에서 Trace하도록.

		FVector TraceEnd = TraceStart + (CrossHairWorldDirection * 10000.f);

		UWorld* World = GetWorld();

		if (World)
		{
			World->LineTraceSingleByChannel(
				TraceHitResult,
				TraceStart,
				TraceEnd,
				ECollisionChannel::ECC_Visibility
			);

			if (!TraceHitResult.bBlockingHit) // 조준하는 곳이 너무 멀리있으면 방향이 이상해지는걸 수정.
			{
				TraceHitResult.ImpactPoint = TraceEnd;
			}

		}

	}


}

void APhase::GetHit_Implementation(const FVector& ImpactPoint)
{

}

void APhase::SetActionState()
{
	switch (ActionState)
	{
	case EActionState::EAS_Unoccupied:
		bUseControllerRotationYaw = false;
		break;
	case EActionState::EAS_Aiming:
		bUseControllerRotationYaw = true;
		break;
	}
}

void APhase::AttachToFlashLight()
{
	if (EquippedFlashLight)
	{
		EquippedFlashLight->Equip(GetMesh(), this, this);
		FlashLightState = EFlashLightState::EFS_EquippedFlashLight;
	}
}

void APhase::AimOffset(float DeltaTime)
{
	
	float Speed = CalculateSpeed();
	bool IsFalling = CharacterMovement->IsFalling();
	
	if (Speed == 0.f && !IsFalling)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f); // 가만히 있는 상태에서 시점을 바꿀때 Yaw
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(StartAimRotation, CurrentAimRotation);
		AO_Yaw = -1.f * DeltaAimRotation.Yaw;

		if (TurnInPlace == ETurnInPlace::ETIP_NotTurning)
		{
			Interp_AO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurningInPlace(DeltaTime);
	}

	if (Speed > 0.f || IsFalling)
	{
		bRotateRootBone = false;
		AO_Yaw = 0.f;
		StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		TurnInPlace = ETurnInPlace::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().GetNormalized().Pitch;
}

float APhase::CalculateSpeed()
{
	FVector Velocity = this->GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size();
}

void APhase::TurningInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurnInPlace = ETurnInPlace::ETIP_TurnRight;
	}
	else if (AO_Yaw < -90.f)
	{
		TurnInPlace = ETurnInPlace::ETIP_TurnLeft;
	}

	if (TurnInPlace != ETurnInPlace::ETIP_NotTurning) //움직이거나 점프일때 
	{
		Interp_AO_Yaw = FMath::FInterpTo(Interp_AO_Yaw, 0.f, DeltaTime, 6.f);
		AO_Yaw = Interp_AO_Yaw; //AO_Yaw 0으로 재설정.
		
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurnInPlace = ETurnInPlace::ETIP_NotTurning;
			StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void APhase::SetOverlappingInteractitem(AInteract* Interact)
{
	InteractItem = Interact;

	FlashLight = Cast<AFlashLight>(InteractItem);

}

void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActionState();

	HideMeshifCameraClose();

	AimOffset(DeltaTime);

	InterpFOV(DeltaTime);
	
	FHitResult TraceHitResult;

	TraceCrossHair(TraceHitResult);
	HitTarget = TraceHitResult.ImpactPoint;

}

void APhase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APhase::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhase::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APhase::Jump);
		EnhancedInputComponent->BindAction(RunPressedAction, ETriggerEvent::Triggered, this, &APhase::RunPressed);
		EnhancedInputComponent->BindAction(RunReleasedAction, ETriggerEvent::Triggered, this, &APhase::RunReleased);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APhase::InteractPressed);
		EnhancedInputComponent->BindAction(FlashOnOffAction, ETriggerEvent::Triggered, this, &APhase::FlashOnOffPressed);
		EnhancedInputComponent->BindAction(AimPressedAction, ETriggerEvent::Triggered, this, &APhase::AimPressed);
		EnhancedInputComponent->BindAction(AimReleasedAction, ETriggerEvent::Triggered, this, &APhase::AimReleased);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APhase::Fire);
	}
}

