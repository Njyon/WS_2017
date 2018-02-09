// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnemySpawnSound.generated.h"

UCLASS()
class MYPROJECT_API AMyEnemySpawnSound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemySpawnSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	class UWorld* world;

	float soundTimeDilation;

	float cleanupDelay = 1.0f;

	FTimerHandle cleanup;

	void Cleanup();

};
