// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HauntedMension/Interact/Interact.h"
#include "KeyPad.generated.h"

class UCameraComponent;
class UTextRenderComponent;
class AKeyActor;
class USoundBase;

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

	void ButtonEnter(AKeyPad* KeyPad, FText Value, bool bConfirmed, bool IsBackSpace);

	void PressBackspace();

	void OnConfirmed();

	UFUNCTION()
	void SetViewPlayerCamera();

protected:

	FTimerHandle CameraHandle;

	FTimerHandle InteractHandle;

	UPROPERTY(EditAnywhere, Category = "Password")
		FText Password;

	UPROPERTY(EditAnywhere, Category = "Password")
		TArray<UChildActorComponent*> ButtonArray;

	UPROPERTY(EditAnywhere, Category = "Password")
		bool bCanEnter = true;
	
	UPROPERTY(EditAnywhere, Category = "Password")
		AActor* KeypadTarget;

	UPROPERTY(EditAnywhere, Category = "Password")
		float CameraBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Password")
		float InteractInterval = 1.f;

	UPROPERTY(EditAnywhere)
		float MaterialInterval = 0.5f;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AInteract> InteractActor;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> AttachActorClass;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> ButtonClickSound;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> CorrectSound;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> WrongSound;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterial> DefaultMaterial;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterial> CorrectMaterial;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UMaterial> WrongMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UTextRenderComponent> EnterPassword;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UStaticMeshComponent> Glass;

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

};
