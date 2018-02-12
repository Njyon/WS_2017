// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Spawn.generated.h"

UCLASS()
class MYPROJECT_API ASpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawn();
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACharacter> enemyToSpawn;

	UPROPERTY()
		bool canSpawn = true;
	UPROPERTY()
		int spawnQue;
	UPROPERTY()
		float spawnDelay = 5.0f;

	UFUNCTION()
		void Spawnenemy(bool first);
	UFUNCTION()
		void Reset();

private:
	FTimerHandle timeHandle;
	//class UWorld* world;

protected:
	virtual void BeginPlay() override;
};
