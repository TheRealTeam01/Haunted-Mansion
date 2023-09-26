
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

	virtual void Fire(FVector& HitTarget);

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

private:	
	
	class APhase* Phase;

	UPROPERTY(EditAnywhere)
	int32 CarriedAmmo;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

public:

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }
};
