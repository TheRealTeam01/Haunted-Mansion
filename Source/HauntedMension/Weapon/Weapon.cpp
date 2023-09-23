#include "Weapon.h"
#include "HauntedMension/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HauntedMension/Interfaces/HitInterface.h"

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

				UGameplayStatics::ApplyDamage(
					HitResult.GetActor(),
					30.f,
					OwnerPawn->Controller,
					this,
					UDamageType::StaticClass()
				);

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
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

