// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectProjectile.generated.h"

UENUM()
enum class collisionSwitch
{
	player,
	npc
};

UCLASS(config=Game)
class AMyProjectProjectile : public AActor
{
	GENERATED_BODY()

		/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = Projectile)
		int projectileDamage = 50;

	UPROPERTY(EditAnywhere, Category = Projectile)
		int headshotMultiplier = 2;

	UPROPERTY(EditAnywhere, Category = Projectile)
		float forceImpulse = 100.0f;

	UPROPERTY(EditAnywhere, Category = Projectile)
		float ressourceRefill = 100.0f;

	int headshotdamage;

	class AMyProjectCharacter* source;

	class ATP_ThirdPersonCharacter* damageNPC;


public:
	AMyProjectProjectile();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHitBpEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemyHitBpEvent();

	UPROPERTY(EditAnywhere, Category = CollisionSwitch)
		collisionSwitch state;

	UFUNCTION()
		void Initialize(AMyProjectCharacter* character);

	UFUNCTION(BlueprintCallable)
		void InitializeNPC(ATP_ThirdPersonCharacter* damageNPC);


	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	virtual void BeginPlay();
};

