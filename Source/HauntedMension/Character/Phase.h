// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "HauntedMension/HMTypes/TurnInPlace.h"
#include "HauntedMension/HMTypes/HMTypes.h"
#include "Phase.generated.h"

UCLASS()
class HAUNTEDMENSION_API APhase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	APhase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetOverlappingInteractitem(class AInteract* Interact);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> Weapon;

	void PlayPickUpMontage();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void Jump(const FInputActionValue& Value);
	
	void RunPressed();

	void RunReleased();

	void HideMeshifCameraClose();

	void InteractPressed();

	void AimPressed();

	void AimReleased();

	void InterpFOV(float DeltaTime);

	void FlashOnOffPressed();

	void PlayReloadMontage();

	void ReloadPressed();

	void FinishReload();

	void FirePressed();
	
	void TraceCrossHair(FHitResult& TraceHitResult);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override; // 블루프린트에서도 호출할거면 Implementaion을 붙임.

	void SetActionState();

	void UpdateHUDAmmo();

	void UpdateHUDCarriedAmmo();

	bool CanFire();

	bool CanReload();

	UFUNCTION(BlueprintCallable)
	void EndPickUp();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FVector HitTarget;

	UPROPERTY(VisibleAnywhere)
	class AInteract* InteractItem;

	UPROPERTY(VisibleAnywhere)
	class AFlashLight* EquippedFlashLight;
	
	UPROPERTY(VisibleAnywhere)
	AWeapon* DefaultWeapon;

	UPROPERTY(VisibleAnywhere)
	class AAmmoPickUp* AmmoPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFlashLightState FlashLightState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EActionState ActionState = EActionState::EAS_Unoccupied;
	void AimOffset(float DeltaTime);

	float CalculateSpeed();

	void TurningInPlace(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	ETurnInPlace TurnInPlace;

	UPROPERTY(BlueprintReadOnly)
	AFlashLight* FlashLight;
	
	UPROPERTY(BlueprintReadOnly)
		bool bAiming;

	class UHMOverlay* HMOverlay;
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

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* AimPressedAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* AimReleasedAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ReloadAction;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* StatComponent;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* PickupMontage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere)
		class USoundBase* NoAmmoSound;

	UPROPERTY(EditAnywhere)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	float CameraDistanceThresHold = 150.f;

	UPROPERTY(EditAnywhere)
		float HP = 100.f;
	
	UPROPERTY(EditAnywhere)
		float MaxHP = 100.f;


	float AO_Yaw;
	float AO_Pitch;
	float Interp_AO_Yaw;

	bool bRotateRootBone;

	FRotator StartAimRotation;

	UFUNCTION(BlueprintCallable)
		void AttachToFlashLight();

	APlayerController* PlayerController;

	class AHMController* HMController;

	float DefaultFOV;

	float CurrentFOV;
	
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 30.f;

public:

	FORCEINLINE float GetAO_Yaw() { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() { return AO_Pitch; }
	FORCEINLINE ETurnInPlace GetTurningInPlace() { return TurnInPlace; }
	FORCEINLINE bool GetbRotateRootBone() { return bRotateRootBone; }
	FORCEINLINE AWeapon* GetDefaultWeapon() { return DefaultWeapon; }
	FORCEINLINE FVector GetHitTarget() { return HitTarget; }
	FORCEINLINE EActionState GetActionState() { return ActionState; }
	FORCEINLINE AAmmoPickUp* GetAmmoPickUp() { return AmmoPickup; }
};
