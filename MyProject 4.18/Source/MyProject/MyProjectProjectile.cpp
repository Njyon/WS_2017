// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProjectProjectile.h"
#include "MyProject/MyProjectCharacter.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AMyProjectProjectile::AMyProjectProjectile() 
{

	world = GetWorld();

	static ConstructorHelpers::FClassFinder<AMyHitWall> HitWallSound(TEXT("'/Game/Blueprints/Player/Behaviour/HitSounds/HitWall'"));
	if (HitWallSound.Class != NULL)
	{
		HitWall = HitWallSound.Class;
	}
	static ConstructorHelpers::FClassFinder<AMyHitNPC> HitNPCSound(TEXT("'/Game/Blueprints/Player/Behaviour/HitSounds/HitNPC'"));
	if (HitNPCSound.Class != NULL)
	{
		HitNPC = HitNPCSound.Class;
	}
	static ConstructorHelpers::FClassFinder<AMyHitHead> HitHeadSound(TEXT("'/Game/Blueprints/Player/Behaviour/HitSounds/HitHead'"));
	if (HitHeadSound.Class != NULL)
	{
		HitHead = HitHeadSound.Class;
	}
	static ConstructorHelpers::FClassFinder<AMyHitSand> HitSandSound(TEXT("'/Game/Blueprints/Player/Behaviour/HitSounds/HitSand'"));
	if (HitSandSound.Class != NULL)
	{
		HitSand = HitSandSound.Class;
	}
	static ConstructorHelpers::FClassFinder<AMyHitMetal> HitMetalSound(TEXT("'/Game/Blueprints/Player/Behaviour/HitSounds/HitMetal'"));
	if (HitMetalSound.Class != NULL)
	{
		HitMetal = HitMetalSound.Class;
	}

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

	SoundSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("SoundSpawn"));
	SoundSpawn->SetupAttachment(RootComponent);
	SoundSpawn->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

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

void AMyProjectProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	
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

	FActorSpawnParameters spawnInfo;
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
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
					
					AMyHitHead* soundhead = world->SpawnActor<AMyHitHead>(
						HitHead,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
					
				
				}

				else if (Hit.GetComponent()->ComponentHasTag("Body"))
				{
					//UE_LOG(LogTemp, Warning, TEXT("hit body"));
					hittedNPC->Damage(projectileDamage);
					OnEnemyHitBpEvent();

					AMyHitNPC* soundbody = world->SpawnActor<AMyHitNPC>(		
						HitNPC,															
						SoundSpawn->GetComponentTransform().GetLocation(),				
						FRotator(0,0,0),																
						spawnInfo);
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

			if (Hit.IsValidBlockingHit() == true)
			{
				if (Hit.GetActor()->ActorHasTag("Metal"))
				{
					AMyHitMetal* metalbodysand = world->SpawnActor<AMyHitMetal>(
						HitMetal,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
				{
					AMyHitWall* soundbodywall = world->SpawnActor<AMyHitWall>(
						HitWall,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else /*if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)*/
				{
					AMyHitSand* soundbodysand = world->SpawnActor<AMyHitSand>(
						HitSand,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}
			}

			Destroy();
			
		}
		else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			OnHitBpEvent();

			if (Hit.IsValidBlockingHit() == true)
			{
				if (Hit.GetActor()->ActorHasTag("Metal"))
				{
					AMyHitMetal* metalbodysand = world->SpawnActor<AMyHitMetal>(
						HitMetal,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
				{
					AMyHitWall* soundbodywall = world->SpawnActor<AMyHitWall>(
						HitWall,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else /*if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)*/
				{
					AMyHitSand* soundbodysand = world->SpawnActor<AMyHitSand>(
						HitSand,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}
			}

			Destroy();
			
		}
		

		break;

	case collisionSwitch::npc:

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * forceImpulse, GetActorLocation());

			if (Hit.IsValidBlockingHit() == true)
			{
				if (Hit.GetActor()->ActorHasTag("Metal"))
				{
					AMyHitMetal* metalbodysand = world->SpawnActor<AMyHitMetal>(
						HitMetal,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
				{
					AMyHitWall* soundbodywall = world->SpawnActor<AMyHitWall>(
						HitWall,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}
				else /*if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)*/
				{
					AMyHitSand* soundbodysand = world->SpawnActor<AMyHitSand>(
						HitSand,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}
			}

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

			if (Hit.IsValidBlockingHit() == true)
			{
				if (Hit.GetActor()->ActorHasTag("Metal"))
				{
					AMyHitMetal* metalbodysand = world->SpawnActor<AMyHitMetal>(
						HitMetal,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
				{
					AMyHitWall* soundbodywall = world->SpawnActor<AMyHitWall>(
						HitWall,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}

				else /*if (Hit.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)*/
				{
					AMyHitSand* soundbodysand = world->SpawnActor<AMyHitSand>(
						HitSand,
						SoundSpawn->GetComponentTransform().GetLocation(),
						FRotator(0, 0, 0),
						spawnInfo);
				}
			}

			Destroy();
			
		}

		break;
	}
}