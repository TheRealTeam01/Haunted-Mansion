
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Weapon.generated.h"

UCLASS()
class HAUNTEDMENSION_API AWeapon : public AActor , public IHitInterface
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void Fire(FVector& HitTarget);

	void Reload();

	void PickUpAmmo(int32 AmmoAmount);

protected:
	
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* FlashParticle;
	
	UPROPERTY(EditAnywhere)
		UParticleSystem* HitFlashParticle;

	UPROPERTY(EditAnywhere)
		class UAnimationAsset* FireAnimation;
	
	UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);

	void UpdateAmmos();

	void SpendAmmo();

private:	
	
	UPROPERTY()
	class APhase* Character;

	UPROPERTY()
	class AHMHUD* HMHUD;

	UPROPERTY()
	class AHMController* HMController;

	UPROPERTY(EditAnywhere)
	int32 CarriedAmmo = 5;

	UPROPERTY(EditAnywhere)
	int32 Ammo = 5;

	UPROPERTY(EditAnywhere)
	int32 MaxAmmo = 5;

	UPROPERTY(EditAnywhere)
		int32 MaxCarriedAmmo = 30;

	UPROPERTY(EditAnywhere)
		float Damage = 30.f;

	UPROPERTY(EditAnywhere)
		float HeadShotDamage = 60.f;
public:

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }
	
	FORCEINLINE int32 GetAmmo() { return Ammo;}
	
	FORCEINLINE int32 GetCarriedAmmo() { return CarriedAmmo; }
	
	FORCEINLINE int32 GetMaxAmmo() { return MaxAmmo; }

	UPROPERTY(EditAnywhere)
		int32 AmmountToPickUp = 5;
};
