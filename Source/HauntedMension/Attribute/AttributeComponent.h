
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

	virtual void CalculateDamage(float Damage);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		float Health;
	
	UPROPERTY(EditAnywhere)
		float MaxHealth;

public:

	FORCEINLINE float GetHealth() { return Health; }
		
};
