// Fill out your copyright notice in the Description page of Project Settings.


#include "HauntedMension/Ai/SevarogActorComponent.h"

// Sets default values for this component's properties
USevarogActorComponent::USevarogActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USevarogActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool USevarogActorComponent::CheckDead()
{
	return false;
}


// Called every frame
void USevarogActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

