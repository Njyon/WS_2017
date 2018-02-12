// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawn.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"


// Sets default values
ASpawn::ASpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//world = GetWorld();
}

// Called when the game starts or when spawned
void ASpawn::BeginPlay()
{
	Super::BeginPlay();

}

void ASpawn::Spawnenemy(bool first)
{
	if (canSpawn == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Spawn"), *GetName());

		canSpawn = false;

		//Sapwn
		FActorSpawnParameters spawnInfo;																		
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATP_ThirdPersonCharacter* enemy = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(enemyToSpawn, GetActorLocation(), GetActorRotation(), spawnInfo);
		if (first)
		{
			enemy->SpawnAudio();
		}
	}
	else if (spawnQue > 0 && canSpawn == false)
	{
		GetWorld()->GetTimerManager().SetTimer(timeHandle, this, &ASpawn::Reset, spawnDelay, false);
	}
}

void ASpawn::Reset()
{
	canSpawn = true;
	if (spawnQue > 0)
		Spawnenemy(false);
}

