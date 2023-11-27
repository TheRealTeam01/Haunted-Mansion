#include "Book.h"
#include "HauntedMension/Character/Phase.h"
#include "HauntedMension/Interact/StoneStatue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

ABook::ABook()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh->SetupAttachment(Root);


}

void ABook::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		TimelineUpdate.BindDynamic(this, &ABook::BookRotate);
		Timeline.AddInterpFloat(CurveFloat, TimelineUpdate);
		TimelineFinish.BindUFunction(this, FName("SetPhysics"));
		Timeline.SetTimelineFinishedFunc(TimelineFinish);
	}

	/*Mesh->OnComponentHit.AddDynamic(this, &ABook::OnHit);*/
}

	
void ABook::StoneStatueInteract()
{
	TArray<AActor*> StoneStatues;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStoneStatue::StaticClass(),StoneStatues); //StoneStatue클래스인 엑터들을 가져와 StoneStatues배열에 담아서 반환.
	for (auto statue : StoneStatues)
	{
		TObjectPtr<AStoneStatue> StoneStatue = Cast<AStoneStatue>(statue);
		if (StoneStatue)
		{
			TScriptInterface<IInteractInterface> Interface = TScriptInterface<IInteractInterface>(StoneStatue);
			if (Interface)
			{
				StoneStatue->Interact();
				UE_LOG(LogTemp, Warning, TEXT("StoneStatueInBook"));
			}
		}
	}
	
}

void ABook::PlayPullOutAnimation()
{
	TObjectPtr<APhase> Character = Cast<APhase>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		TObjectPtr<UAnimInstance> AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && CharacterAnim)
		{
			AnimInstance->Montage_Play(CharacterAnim);
			UE_LOG(LogTemp, Warning, TEXT("CharacterAnim"));
		}
	}
}

void ABook::SetPhysics()
{
	Mesh->SetSimulatePhysics(true);
}

//void ABook::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	if (FallSound)
//	{	
//		UGameplayStatics::SpawnSoundAtLocation(this, FallSound, GetActorLocation(), GetActorRotation());
//	}
//}

void ABook::Interact()
{
	if (!IsRotate) 
	{
		ShowInteractWidget(false);

		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		Timeline.PlayFromStart();

		PlayPullOutAnimation();
		
		GetWorld()->GetTimerManager().SetTimer(BookTimer, [this] {StoneStatueInteract();}, WaitTime, false);

		IsRotate = true;

		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//InteractSphere->DestroyComponent();
	}

	

}

void ABook::BookRotate(float DeltaTime)
{
	/*FRotator Rotation(BookRotation * DeltaTime ,0.f, 0.f );*/
	FVector Location(GetActorLocation().X + BookLocation * DeltaTime, GetActorLocation().Y, GetActorLocation().Z);

	//SetActorRelativeRotation(Rotation);
	SetActorRelativeLocation(Location);
}

void ABook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}


