#include "HauntedMension/Character/Phase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "InputActionValue.h"
#include "HauntedMension/Interact/PickUp/FlashLight.h"
#include "HauntedMension/Interact/Interact.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "HauntedMension/Weapon/Weapon.h"
#include "HauntedMension/HMTypes/HMTypes.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetMathLibrary.h"
#include "HauntedMension/HUD/HMOverlay.h"
#include "HauntedMension/HUD/HMHUD.h"
#include "Blueprint/UserWidget.h"
#include "HauntedMension/Controller/HMController.h"
#include "Kismet/GamePlayStatics.h"
#include "HauntedMension/Interact/PickUp/AmmoPickUp.h"
#include "HauntedMension/Attribute/AttributeComponent.h"
#include "HauntedMension/Interact/Door.h"
#include "HauntedMension/Interact/PickUp/DoorKey.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "Components/TextBlock.h"
#include "Components/PawnNoiseEmitterComponent.h"

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

	StatComponent = CreateDefaultSubobject<UAttributeComponent>("Attributes");

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("Pawn Noise Emitter Component");

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

	InitializeComponent();

}

void APhase::InitializeComponent()
{
	
	HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;
	if (HMController)
	{
		HMController->SetHUDHealth(StatComponent->GetHealthPercent());
		HMController->SetHUDStamina(StatComponent->GetStaminaPercent());
	}

	UpdateHUDAmmo();

	UpdateHUDCarriedAmmo();
}

void APhase::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_Reloading) return;

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
	if (ActionState != EActionState::EAS_Unoccupied) return;

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
	/*UE_LOG(LogTemp, Warning, TEXT("Distance : %f"), DistanceToCamera);*/
	if (DistanceToCamera < CameraDistanceThresHold)
	{
		GetMesh()->SetVisibility(false);
	}
	else
	{
		GetMesh()->SetVisibility(true);
	}
}

void APhase::SetOverlappingInteractitem(AInteract* Interact)
{
	InteractItem = TScriptInterface<IInteractInterface>(Interact);

}

void APhase::InteractPressed()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if(InteractItem)
	{
		InteractItem->Interact();
	}

	/*if (FlashLight)
	{
		PlayPickUpMontage();
		EquippedFlashLight = FlashLight;
		FlashLightState = EFlashLightState::EFS_EquippedFlashLight;
	}

	if (AmmoPickup && DefaultWeapon) 
	{
		PlayPickUpMontage();
		DefaultWeapon->PickUpAmmo(DefaultWeapon->AmmountToPickUp);
		AmmoPickup->Destroy();
	}

	if (Key)
	{
		PlayPickUpMontage();
		Key->Destroy();
		KeyState = EKeyState::EKS_EquippedKey;
	}*/

}

void APhase::AimPressed()
{
	if (DefaultWeapon == nullptr && ActionState != EActionState::EAS_Unoccupied) return;

	bAiming = true;
	ActionState = EActionState::EAS_Aiming;

	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	if (PlayerController)
	{
		HMHUD = Cast<AHMHUD>(PlayerController->GetHUD<AHMHUD>());
		if (HMHUD)
		{
			HMOverlay = HMHUD->GetHMOverlay();
			if (HMOverlay)
			{
				HMOverlay->ShowCrossHair();
			}
		}
	}
}

void APhase::AimReleased()
{
	if (DefaultWeapon == nullptr) return;

	 bAiming = false;
	 ActionState = EActionState::EAS_Unoccupied;

	 PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	 if (PlayerController)
	 {
		 HMHUD = Cast<AHMHUD>(PlayerController->GetHUD<AHMHUD>());
		 if (HMHUD)
		 {
			 HMOverlay = HMHUD->GetHMOverlay();
			 if (HMOverlay)
			 {
				 HMOverlay->HideCrossHair();
			 }
		 }
	 }
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
	if (AnimInstance && PickupMontage && CharacterMovement)
	{
		CharacterMovement->DisableMovement();
		ActionState = EActionState::EAS_PickUp;
		HMController->DisableInput(PlayerController);
		AnimInstance->Montage_Play(PickupMontage);

	}
}

void APhase::PlayReloadMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		if (ReloadMontage && CharacterMovement)
		{
			CharacterMovement->DisableMovement();
			AnimInstance->Montage_Play(ReloadMontage);
		}
	}
	
	FOnMontageEnded MontageEnd;
	MontageEnd.BindWeakLambda(this, [this](UAnimMontage* Animmontage, bool bInterrupted)
		{
			if (bInterrupted) // �����ϴ� �߰��� �ǰ��� ���.
			{
				ActionState = EActionState::EAS_Unoccupied;
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("FinishReload"));
				FinishReload();
			}
		});
	AnimInstance->Montage_SetEndDelegate(MontageEnd, ReloadMontage);
}

void APhase::PlayHitMontage(const FVector& ImpactPoint)
{
	FVector Forward = GetActorForwardVector();
	FVector ToTarget = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	double CosTheta = FVector::DotProduct(Forward, ToTarget);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitMontage)
	{
		if (CosTheta >= 0)
		{
			AnimInstance->Montage_Play(HitMontage);
			AnimInstance->Montage_JumpToSection(FName("Front"), HitMontage);
		}
		else
		{
			AnimInstance->Montage_Play(HitMontage);
			AnimInstance->Montage_JumpToSection(FName("Back"), HitMontage);
		}
	}
}

void APhase::ReloadPressed()
{
	if (DefaultWeapon == nullptr && ActionState != EActionState::EAS_Unoccupied) return;

	if (CanReload())
	{
		ActionState = EActionState::EAS_Reloading;
		PlayReloadMontage();
		DefaultWeapon->Reload();
	}
}

void APhase::FinishReload()
{
	ActionState = EActionState::EAS_Unoccupied;
	if (CharacterMovement) CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);

	UpdateHUDAmmo();
	UpdateHUDCarriedAmmo();
}

void APhase::FirePressed()
{
	if (DefaultWeapon == nullptr || ActionState != EActionState::EAS_Aiming) return;

	if (CanFire())
	{	
		DefaultWeapon->Fire(HitTarget);
	}
	
	bool bEnounghAmmo = DefaultWeapon->GetAmmo() > 0 ? true : false;
	
	if (!bEnounghAmmo) // �Ѿ��� �� ���� ��
	{
		PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			HMHUD = Cast<AHMHUD>(PlayerController->GetHUD<AHMHUD>());
			if (HMHUD)
			{
				HMOverlay = HMHUD->GetHMOverlay();
				if (HMOverlay)
				{
					if (NoAmmoSound)
					{
						UGameplayStatics::PlaySoundAtLocation(DefaultWeapon, NoAmmoSound, DefaultWeapon->GetActorLocation());
					}
					HMOverlay->BlinkText->SetText(FText::FromString("Not Enough Ammo"));
					HMOverlay->PlayBlink();

				}
			}
		}
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
	FVector CrossHairWorldPosition; // ���� ��ǥ
	FVector CrossHairWorldDirection; // ���� 


	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld( //�־��� 2D ȭ�� ���� ��ǥ�� 3D ���� ���� ������ �������� ��ȯ.
		UGameplayStatics::GetPlayerController(this, 0) ,
		CrossHairLocation, 
		CrossHairWorldPosition,
		CrossHairWorldDirection
	);
	
	if (bScreenToWorld)
	{
		FVector TraceStart = CrossHairWorldPosition;

		float DistanceToCharacter = (GetActorLocation() - TraceStart).Size();
		TraceStart += CrossHairWorldDirection * (DistanceToCharacter + 100.f); // ĳ���ͺ��� �տ��� Trace�ϵ���.

		FVector TraceEnd = TraceStart + (CrossHairWorldDirection * 80000.f);

		UWorld* World = GetWorld();

		if (World)
		{
			World->LineTraceSingleByChannel(
				TraceHitResult,
				TraceStart,
				TraceEnd,
				ECollisionChannel::ECC_Visibility
			);

			if (!TraceHitResult.bBlockingHit) // �����ϴ� ���� �ʹ� �ָ������� ������ �̻������°� ����.
			{
				TraceHitResult.ImpactPoint = TraceEnd;
			}
			else
			{
				TraceEnd = TraceHitResult.ImpactPoint;
			}
		}

	}


}

void APhase::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (StatComponent)
	{
		if (StatComponent->IsDead() && DeathMontage)
		{
			Die();
		}
		else
		{
			PlayHitMontage(ImpactPoint);

			OnHealthChanged.Broadcast();

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					GetWorld(),
					HitSound,
					GetActorLocation());
			}

			GetCharacterMovement()->DisableMovement();

			GetWorld()->GetTimerManager().SetTimer(HitHandle, [this]() {GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); }, HitDelay, false);
		}
	}
	
	/*HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;
	if (HMController)
	{
		HMController->SetHUDHealth(StatComponent->GetHealth());
	}*/

	//if (HitSound)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(
	//		GetWorld(),
	//		HitSound,
	//		ImpactPoint);
	//}
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

void APhase::UpdateHUDAmmo()
{
	HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;

	if (HMController)
	{
		HMController->SetHUDAmmo(DefaultWeapon->GetAmmo());
	}
}

void APhase::UpdateHUDCarriedAmmo()
{
	HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;

	if (HMController)
	{
		HMController->SetHUDCarriedAmmo(DefaultWeapon->GetCarriedAmmo());
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
	
	Speed = CalculateSpeed();
	bool IsFalling = CharacterMovement->IsFalling();
	
	if (Speed == 0.f && !IsFalling)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f); // ������ �ִ� ���¿��� ������ �ٲܶ� Yaw
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

void APhase::FallEquipped()
{
	if (EquippedFlashLight && DefaultWeapon)
	{
		FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		EquippedFlashLight->DetachFromActor(DetachRule);
		EquippedFlashLight->GetInteractMesh()->SetSimulatePhysics(true);
		FlashLightState = EFlashLightState::EFS_UnEquippedFlashLight;
		EquippedFlashLight = nullptr;

		DefaultWeapon->DetachFromActor(DetachRule);
		DefaultWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
		DefaultWeapon = nullptr;
	}
}

float APhase::CalculateSpeed()
{
	FVector Velocity = this->GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size();
}

bool APhase::CanFire()
{
	if (DefaultWeapon && DefaultWeapon->GetAmmo())
	{
		return DefaultWeapon->GetAmmo() > 0 && ActionState == EActionState::EAS_Aiming ? true : false;
	}

	return false;
}

bool APhase::CanReload()
{
	if (DefaultWeapon && DefaultWeapon->GetAmmo() >= 0 && DefaultWeapon->GetCarriedAmmo())
	{
		return ActionState == EActionState::EAS_Unoccupied && DefaultWeapon->GetCarriedAmmo() > 0 && DefaultWeapon->GetAmmo() < DefaultWeapon->GetMaxAmmo();
	}

	return false;
}

void APhase::EndPickUp()
{
	if (CharacterMovement)
	{	
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
		ActionState = EActionState::EAS_Unoccupied;

	}
}

void APhase::ReportNoise(USoundBase* Sound, float Volume)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation(), Volume);
	}

	NoiseEmitterComponent->MakeNoise(this, Volume, GetActorLocation());
}

float APhase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	StatComponent->CalculateDamage(DamageAmount);

	return DamageAmount;
}

void APhase::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;

	if (AnimInstance && HMController)
	{
		AnimInstance->Montage_Play(DeathMontage);
		DeathState = EDeathState::EDS_Death;
		
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//ActorHasTag(FName("Dead"));
		Tags.Add(FName(TEXT("Dead")));
		
		HMController->SetHUDDie();
	}
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

	if (TurnInPlace != ETurnInPlace::ETIP_NotTurning) //�����̰ų� �����϶� 
	{
		Interp_AO_Yaw = FMath::FInterpTo(Interp_AO_Yaw, 0.f, DeltaTime, 6.f);
		AO_Yaw = Interp_AO_Yaw; //AO_Yaw 0���� �缳��.
		
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurnInPlace = ETurnInPlace::ETIP_NotTurning;
			StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActionState();

	/*HideMeshifCameraClose();*/

	AimOffset(DeltaTime);

	InterpFOV(DeltaTime);

	FHitResult TraceHitResult;

	TraceCrossHair(TraceHitResult);
	HitTarget = TraceHitResult.ImpactPoint;

	if (DefaultWeapon && DefaultWeapon->GetAmmo() == 0) ReloadPressed();

	if (CalculateSpeed() > 150.f)
	{
		HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;
		if(HMController && StatComponent)
		{
			StatComponent->SpendStamina(DeltaTime);
			HMController->SetHUDStamina(StatComponent->GetStaminaPercent());
		}
	}
	else 
	{
		HMController = HMController == nullptr ? Cast<AHMController>(Controller) : HMController;
		if (HMController && StatComponent)
		{
			StatComponent->RegenStamina(DeltaTime);
			StatComponent->RegenHealth(DeltaTime);
			HMController->SetHUDStamina(StatComponent->GetStaminaPercent());
		}
	}
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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APhase::FirePressed);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APhase::ReloadPressed);

	}
}

