// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SevarogActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAUNTEDMENSION_API USevarogActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USevarogActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	int32 GetAttack() { return Attack; }
	bool CheckDead();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccesss = true))
	float Attack = 100;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccesss = true))
	float Hp = 100;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccesss = true))
	float MaxHp;

public:
	FOnHpChanged OnHpChanged;
};
