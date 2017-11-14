// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyProjectCharacter.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "GameFramework/Actor.h"
#include "MyProjectProjectile.generated.h"

UCLASS(config=Game)
class AMyProjectProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = Projectile)
	int projectileDamage = 50;

	UPROPERTY(EditAnywhere, Category = Projectile)
	float headshotMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, Category = Projectile)
	float forceImpulse = 100.0f;

	int headshotdamage;

public:
	AMyProjectProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

