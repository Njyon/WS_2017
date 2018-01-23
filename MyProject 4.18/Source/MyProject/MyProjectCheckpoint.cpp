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


	/*spawn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn"));
	spawn->SetOnlyOwnerSee(true);
	spawn->SetupAttachment(RootComponent);
	spawn->bCastDynamicShadow = false;
	spawn->CastShadow = false;*/

	// Set as root component
	RootComponent = CollisionComp;

}

void AMyProjectCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	vector = GetActorLocation();
	rotator = GetActorRotation();

	/*vector = spawn->GetComponentLocation();
	rotator = spawn->GetComponentRotation();

	UE_LOG(LogTemp, Warning, TEXT("Spawn Location is %s"), *spawn->GetComponentLocation().ToString());*/

	this->lenght = spawns.Num();
}

void AMyProjectCheckpoint::NextCheckpoint()
{
	if (nextCP != NULL)
	{
		nextCP->ActivateNextCP();
	}
}

  void AMyProjectCheckpoint::OnOverlap(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResult)
{
	AMyProjectCharacter* hittedplayer = Cast<AMyProjectCharacter>(otherActor);
	if ((otherActor != NULL) && (otherActor != this) && hittedplayer != NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("DFG"));
		if (!hasUsed)
		{
			UE_LOG(LogTemp, Warning, TEXT("success"));
			this->hasUsed = true;
			hittedplayer->SetRespawn(vector, rotator);
			OnCheckpoint();
			NextCheckpoint();

			for (int i = 0; i <= this->lenght + 1; i++)
			{
				amountOfEnemys--;

				if (i == this->lenght)
				{
					i = 0;
				}
				if (amountOfEnemys >= 0)
				{
					spawns[i]->spawnQue += 1;
					spawns[i]->Spawnenemy();
				}
				else if (amountOfEnemys <= 0)
				{
					return;
				}
			}

		}
	}
}
