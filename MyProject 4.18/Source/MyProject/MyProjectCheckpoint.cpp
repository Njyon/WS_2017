// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectCheckpoint.h"


// Sets default values
AMyProjectCheckpoint::AMyProjectCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCheckpoint::OnOverlap);
	
	spawn = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn"));

	// Set as root component
	RootComponent = CollisionComp;

}

void AMyProjectCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	vector = spawn->GetComponentTransform().GetLocation();
	rotator = spawn->GetComponentTransform().GetRotation().Rotator();
	/*vector = GetActorLocation();
	rotator = GetActorRotation();*/
}

  void AMyProjectCheckpoint::OnOverlap(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResult)
{
	AMyProjectCharacter* hittedplayer = Cast<AMyProjectCharacter>(otherActor);
	if ((otherActor != NULL) && (otherActor != this) && (otherComp != NULL) && hittedplayer != NULL)
	{
		if (!hasUsed)
		{
			hittedplayer->SetRespawn(vector, rotator);
			this->hasUsed = true;
		}
	}
}
