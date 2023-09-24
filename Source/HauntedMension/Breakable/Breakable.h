// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Breakable.generated.h"

UCLASS()
class HAUNTEDMENSION_API ABreakable : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakable();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;
	
	UPROPERTY(EditAnywhere)
	class UGeometryCollectionComponent* GeometryCollection;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UPROPERTY(EditAnywhere)
		class USoundBase* FractionSound;

private:

};
