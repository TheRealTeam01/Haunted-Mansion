#include "Weapon.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "HauntedMension/Controller/HMController.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/PickUp/AmmoPickUp.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::Fire(FVector& HitTarget)
{
	WeaponMesh->PlayAnimation(FireAnimation, false);
	UWorld* World = GetWorld();
	if (World)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		SetInstigator(OwnerPawn);

		const USkeletalMeshSocket* MuzzleFlashSocket = WeaponMesh->GetSocketByName(FName("MuzzleFlash"));
		if (MuzzleFlashSocket)
		{
			FTransform MuzzleFlashTransform = MuzzleFlashSocket->GetSocketTransform(WeaponMesh);
			FVector TraceStart = MuzzleFlashTransform.GetLocation();
			FVector TraceEnd = TraceStart + (HitTarget - TraceStart) * 1.25f;

			DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 5.f, 0U, 2.f);

			FHitResult HitResult;
			World->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceEnd,
				ECollisionChannel::ECC_Visibility
			);

			UGameplayStatics::SpawnEmitterAtLocation(World,
				FlashParticle,
				MuzzleFlashTransform,
				true
			);

			if (HitResult.GetActor())
			{
				bool bHeadShot = HitResult.BoneName.ToString() == FString("head") ? true : false; // 헤드샷인지 판별 

				if (bHeadShot) // 헤드샷이라면
				{
					UGameplayStatics::ApplyDamage(
						HitResult.GetActor(),
						HeadShotDamage,
						OwnerPawn->Controller,
						this,
						UDamageType::StaticClass()
					);
				}
				else // 바디샷이라면
				{
					UGameplayStatics::ApplyDamage(
						HitResult.GetActor(),
						Damage,
						OwnerPawn->Controller,
						this,
						UDamageType::StaticClass()
					);
				}
				IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
				if (HitInterface)
				{
					HitInterface->Execute_GetHit(HitResult.GetActor(),HitResult.ImpactPoint); //Excute_ - Implementaion으로 선언한 인터페이스 함수 호출(호출할 대상, 호출 함수)
					CreateFields(HitResult.ImpactPoint);
				}

				UGameplayStatics::SpawnEmitterAtLocation(World,
					HitFlashParticle,
					HitResult.ImpactPoint,
					HitResult.ImpactPoint.Rotation(),
					true
				);
			}
		}
	}

	SpendAmmo(); // 총알 소비
}

void AWeapon::UpdateAmmos()
{
	int32 RoomInMag = MaxAmmo - Ammo; // 재장전 할 수 있는 수.

	int32 Least = FMath::Min(RoomInMag, CarriedAmmo); //재장전 할 수 있는 수보다 CarriedAmmo가 작다면 모두 씀.

	int32 AmountToReload = FMath::Clamp(RoomInMag, 0, Least);

	CarriedAmmo = FMath::Clamp(CarriedAmmo - AmountToReload, 0, MaxCarriedAmmo);
	Ammo = FMath::Clamp(Ammo + AmountToReload, 0, MaxAmmo);
}

void AWeapon::SpendAmmo()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MaxAmmo);

	Character = Character == nullptr ? Cast<APhase>(GetOwner()) : Character;
	if (Character)
	{
		HMController = HMController == nullptr ? Cast<AHMController>(Character->Controller) : HMController;
		if (HMController)
		{
			HMController->SetHUDAmmo(Ammo);
		}
	}
}

void AWeapon::PickUpAmmo(int32 AmmoAmount)
{

	CarriedAmmo = FMath::Clamp(CarriedAmmo + AmmoAmount, 0, MaxCarriedAmmo);

	Character = Character == nullptr ? Cast<APhase>(GetOwner()) : Character;
	if (Character && Character->GetAmmoPickUp())
	{
		Character->PlayPickUpMontage();
		Character->GetAmmoPickUp()->Destroy();
		HMController = HMController == nullptr ? Cast<AHMController>(Character->Controller) : HMController;
		if (HMController)
		{
			HMController->SetHUDCarriedAmmo(CarriedAmmo);
		}
	}
}

void AWeapon::Reload()
{
	if (Ammo == MaxAmmo || CarriedAmmo == 0) return;
	
	UpdateAmmos();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

