// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HauntedMension/Interfaces/HitInterface.h"
#include "Breakable.generated.h"

class UCapsuleComponent;
class UGeometryCollectionComponent;
class USoundBase;
class AInteract;
UCLASS()
class HAUNTEDMENSION_API ABreakable : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	

	ABreakable();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual void SpawnItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FractionSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AInteract> SpawnItems;

	UPROPERTY(EditAnywhere)
	float LifeSpan = 1.f;
private:

};
