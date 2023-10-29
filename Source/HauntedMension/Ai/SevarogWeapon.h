// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Weapon/Weapon.h"
#include "SevarogWeapon.generated.h"

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API ASevarogWeapon : public AWeapon
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Swing();

private:
};
