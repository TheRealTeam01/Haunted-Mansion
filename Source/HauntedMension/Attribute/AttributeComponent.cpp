#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAttributeComponent::CalculateDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + RegenStaminaCost * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::SpendStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina - RegenStaminaCost * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}