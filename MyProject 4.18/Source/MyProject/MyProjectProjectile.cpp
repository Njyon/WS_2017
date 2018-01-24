// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProjectProjectile.h"
#include "MyProject/MyProjectCharacter.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AMyProjectProjectile::AMyProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

}

void AMyProjectProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (state == collisionSwitch::player)
	{
		this->CustomTimeDilation = 1;
	}
}

void AMyProjectProjectile::Initialize(AMyProjectCharacter* character)
{
	this->source = character;
}
void AMyProjectProjectile::InitializeNPC(ATP_ThirdPersonCharacter* damageNPC)
{
	this->damageNPC = damageNPC;
}

void AMyProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMyProjectCharacter* hittedplayer = Cast<AMyProjectCharacter>(OtherActor);
	ATP_ThirdPersonCharacter* hittedNPC = Cast<ATP_ThirdPersonCharacter>(OtherActor);

	
	// Only add impulse and destroy projectile if we hit a physics

	switch (state)
	{
	case collisionSwitch::player:
		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && hittedNPC != NULL)
		{
			if (Hit.GetComponent() != NULL)
			{
				//UE_LOG(LogTemp, Warning, TEXT("BoneName: %s"), *Hit.BoneName.ToString());
				if (Hit.GetComponent()->ComponentHasTag("Head"))
				{
					UE_LOG(LogTemp, Warning, TEXT("hit head"));
					headshotdamage = projectileDamage * headshotMultiplier;
					source->RessoourceRefill(ressourceRefill);
					hittedNPC->Damage(headshotdamage);
					//source = GetWorld()->GetFirstPlayerController();
					OnEnemyHitBpEvent();
				
				}

				else if (Hit.GetComponent()->ComponentHasTag("Body")/* || Hit.GetComponent()->ComponentHasTag("LLeg") || Hit.GetComponent()->ComponentHasTag("RLeg")*/)
				{
					//UE_LOG(LogTemp, Warning, TEXT("hit body"));
					hittedNPC->Damage(projectileDamage);
					OnEnemyHitBpEvent();
				}

				else
				{
					OnHitBpEvent();
					Destroy();
				}

				//ProjectileMovement->bShouldBounce = false;
				Destroy();
			}
		}
		else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			OnHitBpEvent();
			OtherComp->AddImpulseAtLocation(GetVelocity() * forceImpulse, GetActorLocation());

			Destroy();
		}
		else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			OnHitBpEvent();
			Destroy();
		}
		

		break;

	case collisionSwitch::npc:

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * forceImpulse, GetActorLocation());
			OnHitBpEvent();
			Destroy();
		}
		else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && hittedplayer != NULL)
		{
			//UE_LOG(LogTemp, Warning, TEXT("hit"));
			hittedplayer->Damage(projectileDamage, damageNPC->GetActorTransform().GetLocation());

			//ProjectileMovement->bShouldBounce = false;
			Destroy();
		}
		else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			OnHitBpEvent();
			Destroy();
		}

		break;
	}
}