#include "HauntedMension/Character/Phase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "InputActionValue.h"
#include "HauntedMension/PickUp/FlashLight.h"
#include "HauntedMension/Interact/Interact.h"


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
}

void APhase::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	GetMesh()->HideBoneByName(FName("teddy_bear_root"), EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(FName("heart_box"), EPhysBodyOp::PBO_None);
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
	if ((Camera->GetComponentLocation() - GetActorLocation()).Size() < CameraDistanceThresHold)
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
	AFlashLight* FlashLight = Cast<AFlashLight>(InteractItem);
	if (FlashLight)
	{
		FlashLight->Equip(GetMesh(), this, this);
		EquippedFlashLight = FlashLight;
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

void APhase::SetOverlappingInteractitem(AInteract* Interact)
{
	InteractItem = Interact;

}

void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HideMeshifCameraClose();
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
	}
}

