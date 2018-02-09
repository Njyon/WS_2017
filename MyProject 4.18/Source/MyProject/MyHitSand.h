// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHitSand.generated.h"

UCLASS()
class MYPROJECT_API AMyHitSand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyHitSand();

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	//HitSand
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* HitSandAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* HitSandAudioComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float soundTimeDilation;

	class UWorld* world;

	float cleanupDelay = 1.0f;

	FTimerHandle cleanup;

	void Cleanup();

	
	
};
