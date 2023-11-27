#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyActor.generated.h"

class UTextRenderComponent;
class AKeyPad;

UCLASS()
class HAUNTEDMENSION_API AKeyActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AKeyActor();

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void ButtonClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	
	UFUNCTION()
	void OnCursor(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void EndCursor(UPrimitiveComponent* TouchedComponent);

	void ResetButton();

	TObjectPtr<AKeyPad> KeyPad;

	FORCEINLINE void SetKeyPad(AKeyPad* KeyPadActor) { KeyPad = KeyPadActor; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Key;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextRenderComponent> KeyCode;

	UPROPERTY(EditAnywhere)
		FText Number;

	UPROPERTY(EditAnywhere)
		bool bConfirmed = false;
	
	UPROPERTY(EditAnywhere)
		bool IsBackSpace = false;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterial> BeginHoverMaterial;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterial> EndHoverMaterial;


};
