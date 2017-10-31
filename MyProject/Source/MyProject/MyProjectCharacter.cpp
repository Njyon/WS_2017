// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

///Unreal Stuff
#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "PhysicsEngine/BodyInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "ConstructorHelpers.h"
///Our Stuff
#include "MyProject/MyProjectProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	movementComponent = GetCharacterMovement();
	world = GetWorld();

}

			//////////////////////////////////////
			//////////	  Begin Play	//////////
			//////////////////////////////////////

void AMyProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor

	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	this->movementComponent->SetPlaneConstraintEnabled(true);
}

			//////////////////////////////////////
			//////////	Input Bind		//////////
			//////////////////////////////////////

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* playerInputComponent)
{
	// set up gameplay key bindings
	check(playerInputComponent);

	//playerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//playerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	playerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	playerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);

	/// Input Left Mouse Button
	playerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProjectCharacter::LMBPressed);
	playerInputComponent->BindAction("Fire", IE_Released, this, &AMyProjectCharacter::LMBReleased);
	/// Input Right Mouse Button
	playerInputComponent->BindAction("Slomo", IE_Pressed, this, &AMyProjectCharacter::RMBPressed);
	playerInputComponent->BindAction("Slomo", IE_Released, this, &AMyProjectCharacter::RMBReleased);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	//playerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//playerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	//playerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//playerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);
}

			//////////////////////////////////////
			//////////		TICK		//////////
			//////////////////////////////////////

void AMyProjectCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	///Shoot
	if (isLMBPressed == true)
	{
		if (this->isBulletFired == false)
		{
			this->isBulletFired = true;
			SpawnBullet();		// Shoot a Bullet
		}
		else
		{
			///InCrease Shoot speed when Slomo is Active
			if (isSlomoActive)
			{
				if (world->GetTimerManager().IsTimerActive(timeHandle) == true && isShootingInNormalSpeed == true)
				{
					isShootingInNormalSpeed = false;
					world->GetTimerManager().ClearTimer(timeHandle); // Cleartimer if u switch into Slomo mode
				}
				else if (world->GetTimerManager().IsTimerActive(timeHandle) == false) // Trigger Only Once and than wait for Cooldown (Cooldwon is Rewriteble)
				{
					world->GetTimerManager().SetTimer(timeHandle, this, &AMyProjectCharacter::BulletCooldown, fireRateSlomo, false); // Shoot Cooldown
				}
			}
			///Shoot Speed = Normal
			else
			{
				if (world->GetTimerManager().IsTimerActive(timeHandle) == false) // Trigger Only Once and than wait for Cooldown (Cooldwon is Rewriteble)
				{
					world->GetTimerManager().SetTimer(timeHandle, this, &AMyProjectCharacter::BulletCooldown, firerateNoSlomo, false); // Shoot Cooldown

					if (isShootingInNormalSpeed == false)
					{
						isShootingInNormalSpeed = true;
					}
				}
			}
		}
	}

	///Slomo
	if (isSlomoActive == true)
	{
		if (movementComponent->IsFalling() == false /* and / or Sliding  */ )  // check if Character is in "Action"
		{
			isSlomoActive = false;
			UGameplayStatics::SetGlobalTimeDilation(world, 1); // Set Time Dilation to Normal
		}
	}
}

			//////////////////////////////////////
			//////////	    Input       //////////
			//////////////////////////////////////

void AMyProjectCharacter::LMBPressed()
{
	isLMBPressed = true;
}

void AMyProjectCharacter::LMBReleased()
{
	isLMBPressed = false;
}

void AMyProjectCharacter::RMBPressed()
{
	if (movementComponent->IsFalling() == true /* and / or Slinding */ ) // Check if you can Set Slomo to Active
	{
		isSlomoActive = true;
		
		UGameplayStatics::SetGlobalTimeDilation(world, slomoTimeDilation); // Set Time to Slomo Time Dilation
	}
}

void AMyProjectCharacter::RMBReleased()
{
	isSlomoActive = false;
	UGameplayStatics::SetGlobalTimeDilation(world, 1); // Set Time to Noraml
}

				//////////////////////////////////////
				//////////	  Functions     //////////
				//////////////////////////////////////

void AMyProjectCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), value);
		this->VAxis = value;
	}
}

void AMyProjectCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), value);
		this->HAxis = value;
	}
}

void AMyProjectCharacter::TurnAtRate(float rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void AMyProjectCharacter::BulletCooldown()
{
	this->isBulletFired = false;
}

void AMyProjectCharacter::SpawnBullet()
{
	////////////////////////////////////////////////
	// Cast A Ray form the Camera to the Crosshair
	////////////////////////////////////////////////

	FVector camPos = FirstPersonCameraComponent->GetComponentTransform().GetLocation(); // Get Camera Position (World)

	FVector camForwardVec = FirstPersonCameraComponent->GetComponentTransform().GetRotation().GetForwardVector(); //Get Forward Vector from Camera
	camForwardVec = camForwardVec * 50000; // Make the Vector longer

	FVector endRaycastPos = camPos + camForwardVec; // Get a EndPosition for the Raycast

	FCollisionQueryParams fireTraceParams = FCollisionQueryParams(FName(TEXT("CamTrace")), true, this);		// Params for the RayCast
	fireTraceParams.bTraceComplex = false;
	fireTraceParams.bTraceAsyncScene = false;
	fireTraceParams.bReturnPhysicalMaterial = false;
	FHitResult hitResult(ForceInit);


	world->LineTraceSingleByChannel(	// Raycast
		hitResult,						// Result
		camPos,							// RayStart
		endRaycastPos,					// RayEnd
		ECC_Pawn, 
		fireTraceParams);

	if (hitResult.IsValidBlockingHit() == true)																		// Does the RayHit?
	{
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(												// Set a Rotater from MuzzelLocation to Ray Imapact Point
			FP_MuzzleLocation->GetComponentTransform().GetLocation(),												// Get Muzzel Location in World
			hitResult.ImpactPoint);																					// Get Impact Point in World

		FActorSpawnParameters spawnInfo;																			// Spawn Info
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;					// Spawn Actor Always
		spawnInfo.Owner = this;																						// Owner of Spawned Actor is this Class
		spawnInfo.Instigator = this;																				// if you spawn a projectile and pass over instigator as say, your pawn. then when the projectile wants to do damage, you can pass it over to the damage function. So you know who fire the projectile, you can then decide if you want friendly fire, and all sorts of things can be done after you know this bit of info.

		AMyProjectProjectile* projectile = world->SpawnActor<AMyProjectProjectile>(		// Spawn the Bullet
			playerProjectile,															// SubClass
			FP_MuzzleLocation->GetComponentTransform().GetLocation(),					// SpawnLocation
			newRotation,																// SpawnRotation
			spawnInfo);																	// Set Spawn Info
	}

	////////////////////////////////////////////////
	////////////	 Play Sound and Animation
	////////////////////////////////////////////////

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}


