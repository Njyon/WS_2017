// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemySpawnSound.h"


// Sets default values
AMyEnemySpawnSound::AMyEnemySpawnSound()
{
	world = GetWorld();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemySpawnSound::BeginPlay()
{
	Super::BeginPlay();

	//SpawnAudioComponent->Play();

	//GetWorld()->GetTimerManager().SetTimer(cleanup, this, &AMyEnemySpawnSound::Cleanup, cleanupDelay, false);
}

void AMyEnemySpawnSound::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called every frame
void AMyEnemySpawnSound::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMyEnemySpawnSound::Cleanup()
{
	Destroy();
}