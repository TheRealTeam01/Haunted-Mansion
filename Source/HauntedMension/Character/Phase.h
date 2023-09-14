// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HauntedMension/Interfaces/InteractInterface.h"
#include "Phase.generated.h"

UCLASS()
class HAUNTEDMENSION_API APhase : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	APhase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetOverlappingInteractitem(class AInteract* Interact);

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void Jump(const FInputActionValue& Value);
	
	void RunPressed();

	void RunReleased();

	void HideMeshifCameraClose();

	void InteractPressed();

	void FlashOnOffPressed();

	UPROPERTY(VisibleAnywhere)
	class AInteract* InteractItem;

	class AFlashLight* EquippedFlashLight;

private:

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* RunPressedAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* RunReleasedAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* FlashOnOffAction;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(EditAnywhere)
	float CameraDistanceThresHold = 150.f;


};
