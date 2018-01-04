// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectCharacter.h"
#include "MyProjectCheckpoint.generated.h"


UCLASS(config = Game)
class MYPROJECT_API AMyProjectCheckpoint : public AActor
{
	GENERATED_BODY()
	
	
	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class UBoxComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		class USceneComponent* spawn;

public:	
	// Sets default values for this actor's properties
	AMyProjectCheckpoint();

	FVector vector;
	FRotator rotator;
	bool hasUsed = false;

	///** Returns CollisionComp subobject **/
	FORCEINLINE class UBoxComponent* GetCollisionComp() const { return CollisionComp; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	
	
};
