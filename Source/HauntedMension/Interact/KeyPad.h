// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "KeyPad.generated.h"

class UCameraComponent;
class UTextRenderComponent;

/**
 * 
 */
UCLASS()
class HAUNTEDMENSION_API AKeyPad : public AInteract
{
	GENERATED_BODY()
	
public:

	AKeyPad();

	virtual void BeginPlay() override;

	virtual void Interact();

	void Initiate();


protected:

	UPROPERTY(EditAnywhere, Category = "Password")
		FText Password;

	UPROPERTY(EditAnywhere, Category = "Password")
		TArray<UChildActorComponent*> ButtonArray;

	UPROPERTY(EditAnywhere, Category = "Password")
		bool bCanEnter = false;

	UPROPERTY(EditAnywhere, Category = "Password")
		AActor* KeypadTarget;

	UPROPERTY(EditAnywhere, Category = "Password")
		float CameraBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UTextRenderComponent> EnterCodeText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<USceneComponent> Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor4;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor6;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor7;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor8;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor9;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> KeyActor0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> BackspaceKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UChildActorComponent> EnterKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UCameraComponent> InteractCamera;
};
