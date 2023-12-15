#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SequenceTriggerBox.generated.h"

class ULevelSequencePlayer;
class ULevelSequence;
class UBoxComponent;

UCLASS()
class HAUNTEDMENSION_API ASequenceTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ASequenceTriggerBox();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ShowSKeleton();

	UPROPERTY(EditAnywhere)
		TObjectPtr<ULevelSequence> LevelSequence;
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> PlayerCharacter;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SequecneSkeleton1;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SequecneSkeleton2;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> PlaySkeleton1;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> PlaySkeleton2;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> PlaySkeleton3;

public:	

};
