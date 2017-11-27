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
	
	/** Sphere collision component */
	/*UPROPERTY(VisibleDefaultsOnly, Category = Checkpoint)
	class UBoxComponent* CollisionComp;*/

public:	
	// Sets default values for this actor's properties
	AMyProjectCheckpoint();

	//UFUNCTION()
	//	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	///** Returns CollisionComp subobject **/
	//FORCEINLINE class UBoxComponent* GetCollisionComp() const { return CollisionComp; }

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	
	
};
