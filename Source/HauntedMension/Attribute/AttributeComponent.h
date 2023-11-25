
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAUNTEDMENSION_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health = 100.f;

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere)
		float Stamina = 100.f;

	UPROPERTY(EditAnywhere)
		float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere)
		float RegenStaminaCost = 5.f;
	
	UPROPERTY(EditAnywhere)
		float RegenHealthCost = 10.f;

private:



public:

	FORCEINLINE float GetHealth() { return Health; }

	float GetHealthPercent();

	float GetStaminaPercent();

	void CalculateDamage(float Damage);

	void RegenStamina(float DeltaTime);

	void RegenHealth(float DeltaTime);

	void SpendStamina(float DeltaTime);

	bool IsDead();
};
