#include "Weapon.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "HauntedMension/Controller/HMController.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Interact/PickUp/AmmoPickUp.h"

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

			/*DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 5.f, 0U, 2.f);*/

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
				bool bHeadShot = HitResult.BoneName.ToString() == FString("head") ? true : false; // ��弦���� �Ǻ� 

				if (bHeadShot) // ��弦�̶��
				{
					UGameplayStatics::ApplyDamage(
						HitResult.GetActor(),
						HeadShotDamage,
						OwnerPawn->Controller,
						this,
						UDamageType::StaticClass()
					);
				}
				else // �ٵ��̶��
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
					HitInterface->Execute_GetHit(HitResult.GetActor(),HitResult.ImpactPoint); //Excute_ - Implementaion���� ������ �������̽� �Լ� ȣ��(ȣ���� ���, ȣ�� �Լ�)
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

	SpendAmmo(); // �Ѿ� �Һ�
}

void AWeapon::UpdateAmmos()
{
	int32 RoomInMag = MaxAmmo - Ammo; // ������ �� �� �ִ� ��.

	int32 Least = FMath::Min(RoomInMag, CarriedAmmo); //������ �� �� �ִ� ������ CarriedAmmo�� �۴ٸ� ��� ��.

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
	if (Character)
	{
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

