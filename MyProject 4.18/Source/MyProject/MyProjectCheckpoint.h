// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectCharacter.h"
#include "Spawn.h"
#include "MyProjectCheckpoint.generated.h"


UCLASS(config = Game)
class MYPROJECT_API AMyProjectCheckpoint : public AActor
{
	GENERATED_BODY()
	
	
	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class UBoxComponent* CollisionComp;
	/*UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class UStaticMeshComponent* spawn;*/

public:	
	// Sets default values for this actor's properties
	AMyProjectCheckpoint();

	//Set Spawn points for enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawns)
		TArray<ASpawn*> spawns;

	//Enemies to Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawns)
		int amountOfEnemys;
	
	//Set the Next Checkpoint to highlight upon entering this Checkpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Checkpoint)
		class AMyProjectCheckpoint* nextCP;

	UFUNCTION(BlueprintImplementableEvent)
		void OnCheckpoint();
	
	UFUNCTION()
		void NextCheckpoint();

	UFUNCTION(BlueprintImplementableEvent)
		void ActivateNextCP();

	//Is this the first Checkpoint
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool firstCP;


	FVector vector;
	FRotator rotator;
	bool hasUsed = false;

	//CheckpointSound
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* CheckpointSoundAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* CheckpointSoundAudioComponent;

	///** Returns CollisionComp subobject **/
	FORCEINLINE class UBoxComponent* GetCollisionComp() const { return CollisionComp; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:

	UFUNCTION()
		void OnOverlap(
			class UPrimitiveComponent* hitComp,
			class AActor* otherActor,
			class UPrimitiveComponent* otherComp,
			int32 otherBodyIndex,
			bool fromSweep,
			const FHitResult & sweepResult
		);

	UPROPERTY()
		uint8 lenght;
	
	
};
