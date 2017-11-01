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
#include "Engine.h"
#include "Components/TimelineComponent.h"
///Our Stuff
#include "MyProject/MyProjectProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{

			//////////////////////////////////////
			//////////		Sounds		//////////
			//////////////////////////////////////

	//ShootSound
	/*static ConstructorHelpers::FObjectFinder<USoundCue> ShootCue(TEXT("'/Game/Characters/FirstPerson/Audio/Guns/FirstPersonTemplateWeaponFireShmorph_Cue'"));
	ShootAudioCue = ShootCue.Object;
	ShootAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootAudioComp"));
	ShootAudioComponent->bAutoActivate = false;
	ShootAudioComponent->SetupAttachment(RootComponent);*/

			////////////End Sounds////////////////

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
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	movementComponent = GetCharacterMovement();					// Get the CharacterMovement Component
	world = GetWorld();											// Get World

	// Main Wallrun detector
	wallDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("WallDetector"));
	wallDetector->SetupAttachment(RootComponent);
	wallDetector->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::OnWallDetected);
	wallDetector->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::EndWallDetected);

	// Right Wallrun detector
	wallRightDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("WallRightDetector"));
	wallRightDetector->SetupAttachment(RootComponent);
	wallRightDetector->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::OnRightWallDetected);
	wallRightDetector->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::EndRightWallDetected);

	// Left Wallrun detector
	wallLeftDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("WallLeftDetector"));
	wallLeftDetector->SetupAttachment(RootComponent);
	wallLeftDetector->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::OnLeftWallDetected);
	wallLeftDetector->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::EndLeftWallDetected);

	// Main Wallrun Timeline
	wallrunTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WallrunTimeline"));
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

	// Declare Delegate function to be binded with wallrunFloatReturn(float value)
	FOnTimelineFloat WallrunUpdate;

	WallrunUpdate.BindUFunction(this, FName("WallrunFloatReturn"));

	// Check if Curve asset it valid
	if (this->wallrunCurve)
	{
		wallrunTimeline->AddInterpFloat(this->wallrunCurve, WallrunUpdate, FName("wallrunTimeline"));

		wallrunTimeline->SetLooping(true);
		wallrunTimeline->SetIgnoreTimeDilation(false);
	}

	this->helperWallJumpNegativeFloat = this->wallJumpForce * 2;
	this->helperWallJumpNegativeFloat = this->wallJumpForce - this->helperWallJumpNegativeFloat;
}

			///////////////////////////////////////
			/////////// Post Processing  //////////
			///////////////////////////////////////

void AMyProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*if (ShootAudioCue->IsValidLowLevelFast())
	{
		ShootAudioComponent->SetSound(ShootAudioCue);
	}*/
}


			//////////////////////////////////////
			//////////	Input Bind		//////////
			//////////////////////////////////////

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* playerInputComponent)
{
	// set up gameplay key bindings
	check(playerInputComponent);

	/// Input Jump
	playerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyProjectCharacter::Jump);
	playerInputComponent->BindAction("Jump", IE_Released, this, &AMyProjectCharacter::EndJumping);
	/// Input WASD
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
	playerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	//playerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	playerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);
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
			if (isSlomoActive == true)
			{
				if (world->GetTimerManager().IsTimerActive(timeHandle) == true && isShootingInNormalSpeed == true)
				{
					isShootingInNormalSpeed = false;
					world->GetTimerManager().PauseTimer(timeHandle);
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

							// Mouse //

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

						// Keyboard WASD //

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

						// Keyboard SpaceBar //

/// Jump
void AMyProjectCharacter::Jump()
{
	if (this->movementComponent->IsMovingOnGround() == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		this->LaunchCharacter(									// Launch Character (Jump Up)
			FVector(0.0f, 0.0f, this->jumpHeight),				// Where to Launch
			false,												// XY Override
			true												// Z Override
		);
	}
	else if (this->isWallRight == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		FVector forwardVector = this->GetActorForwardVector() * this->wallJumpForceForward;
		FVector sideVector = this->GetActorRightVector() * this->helperWallJumpNegativeFloat;
		FVector launchVector = sideVector + forwardVector;

		this->LaunchCharacter(FVector(
			launchVector.X,
			launchVector.Y, 
			this->jumpHeightOnWall),
			false, 
			true
		);
	}
	else if (this->isWallLeft == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		FVector forwardVector = this->GetActorForwardVector() * this->wallJumpForceForward;
		FVector sideVector = this->GetActorRightVector() * this->wallJumpForce;
		FVector launchVector = sideVector + forwardVector;

		this->LaunchCharacter(FVector(
			launchVector.X,
			launchVector.Y,
			this->jumpHeightOnWall),
			false,
			true
		);
	}
	//else if(this->isClimb == true)
}

void AMyProjectCharacter::EndJumping()
{
	this->StopJumping();
}

void AMyProjectCharacter::Landed(const FHitResult& hit) 
{
	Super::Landed(hit);

	this->MakeNoise(				// Make Noise for AI Perception
		1.0f,						// Loudness
		this,						// Instigator (PAWN)
		hit.Location,				// Noise Location
		0.0f						// Max Range
	);

	this->WallrunEnd();

}


				//////////////////////////////////////
				//////////	  Functions     //////////
				//////////////////////////////////////

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
		UE_LOG(LogTemp, Warning, TEXT("HA"));

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
	else																				// Ray does not hit anything
	{
		UE_LOG(LogTemp, Warning, TEXT("FUCK"));

		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(												// Set a Rotater from MuzzelLocation to Ray Imapact Point
			FP_MuzzleLocation->GetComponentTransform().GetLocation(),												// Get Muzzel Location in World
			hitResult.TraceEnd);																					// Get Trace end in World

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

// Wallrun (MovePlayer)
void AMyProjectCharacter::WallrunLaunch()
{
	UE_LOG(LogTemp, Warning, TEXT("UPDATE"));

	FVector rayStart = this->FirstPersonCameraComponent->GetComponentTransform().GetLocation();
	FVector rayEnd = this->FirstPersonCameraComponent->GetComponentTransform().GetRotation().GetForwardVector() * 1000 + rayStart;


	FCollisionQueryParams rayParams = FCollisionQueryParams(FName(TEXT("WallRightParam")), true, this);		// Params for the RayCast
	rayParams.bTraceComplex = false;
	rayParams.bTraceAsyncScene = false;
	rayParams.bReturnPhysicalMaterial = false;

	FHitResult hitResult(ForceInit);

	world->LineTraceSingleByChannel(			// Raycast
		hitResult,								// Result
		rayStart,								// RayStart
		rayEnd,									// RayEnd
		ECC_Pawn,
		rayParams);

	if (this->isWallRight == true)
	{
		if (hitResult.IsValidBlockingHit() == true)
		{
			if (hitResult.GetActor()->ActorHasTag("RunWall"))	// Wallrun if you look at wall && the wall is on your Right side
			{
				FVector playerPosition = this->GetTransform().GetLocation();
				this->playerDirection = this->playerDirection * 1000;

				FVector launchCharacterVector = this->wallRunDirection - playerPosition;
				launchCharacterVector = launchCharacterVector + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);

				GravitationOff();
			}
			else												// wallrun if u dont look at wall && the wall is on your Right side
			{
				this->playerDirection = this->playerDirection * 1000;
				FVector launchCharacterVector = this->playerRightVector * 1000 + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);

				GravitationOff();
			}
		}
		else
		{
			this->playerDirection = this->playerDirection * 1000;
			FVector launchCharacterVector = this->playerRightVector * 1000 + this->playerDirection;

			this->LaunchCharacter(launchCharacterVector, true, true);

			GravitationOff();
		}
	}
	else
	{
		if (hitResult.IsValidBlockingHit() == true)
		{
			if (hitResult.GetActor()->ActorHasTag("RunWall"))	// Wallrun if you look at wall && the wall is on your Left side
			{
				FVector playerPosition = this->GetTransform().GetLocation();
				this->playerDirection = this->playerDirection * 1000;

				FVector launchCharacterVector = this->wallRunDirection - playerPosition;
				launchCharacterVector = launchCharacterVector + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);

				GravitationOff();
			}
			else
			{
				this->playerDirection = this->playerDirection * 1000;
				FVector launchCharacterVector = this->playerRightVector * -1000 + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);

				GravitationOff();
			}
		}
		else
		{
			this->playerDirection = this->playerDirection * 1000;
			FVector launchCharacterVector = this->playerRightVector * -1000 + this->playerDirection;

			this->LaunchCharacter(launchCharacterVector, true, true);

			GravitationOff();
		}
	}
}

//// Called after LaunchCharacter
void AMyProjectCharacter::GravitationOff()
{
	this->movementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
	this->movementComponent->GravityScale = 0.2f;
	this->movementComponent->AirControl = 0.0;

	this->wallrunTimeline->Stop();

	world->GetTimerManager().SetTimer(timeHandle, this, &AMyProjectCharacter::WallrunRetriggerableDelay, this->wallrunDuration, false);

}

void AMyProjectCharacter::WallrunRetriggerableDelay()
{
	this->isOnWall = false;

	if (this->wallrunDoOnce == true)
	{
		this->wallrunDoOnce = false;

		if (this->movementComponent->IsFalling() == true)
		{
			this->isWallRight = false;
			this->isWallLeft = false;
		}
	}
}


// Resets Values
void AMyProjectCharacter::WallrunEnd()
{
	this->movementComponent->GravityScale = this->gravitation;
	this->movementComponent->AirControl = this->airControll;
	this->movementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 0.0f));
	this->isOnWall = false;
	this->isWallRight = false;
	this->isWallLeft = false;
	this->wallrunDoOnce = true;
}

						//////////////////////////////////////
						//////////	  Collision     //////////
						//////////////////////////////////////

/// Main Start Walldetection
void AMyProjectCharacter::OnWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResul)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() /*&& Wallclimb false */)		// Check if the Wall has the right TAG and if the player is in the air
	{
		this->isOnWall = true;																					// Set the Wallrun State
		this->wallCollisionCounter = 0;																			// Set the Wallruncounter back to 0 (Colliding bug prevention) 
		this->wallCollisionCounter++;																			// Count the Wallruncounter 1 up
		this->playerDirection = FirstPersonCameraComponent->GetForwardVector();									// safe the Playerdirection for the CharacterLaunch
		this->playerRightVector = FirstPersonCameraComponent->GetRightVector();									// safe the PlayersRightVector for the ChracterLaunch
		
		this->wallrunTimeline->Play();																			// Start the Timeline
	}
}
/// Main End Walldetection
void AMyProjectCharacter::EndWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling())
	{
		this->wallrunTimeline->Stop();

		if (this->wallCollisionCounter >= 1)
		{
			this->wallCollisionCounter--;

			if (this->wallCollisionCounter == 0)
			{
				this->WallrunEnd();
			}
		}
	}
}

/// Right Walldetector Begin
void AMyProjectCharacter::OnRightWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResul)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() /* && Wallclimb false */)
	{
		this->isWallLeft = false;					// set the bool false so it only can on of them be true (safty first)
		this->isWallRight = true;					

	}
}
/// Right Walldetector End
void AMyProjectCharacter::EndRightWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() /* && Wallclimb false */)
	{
		this->isWallLeft = false;					
		this->isWallRight = false;

	}
}

/// Left Walldetector Begin
void AMyProjectCharacter::OnLeftWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResul)
{

	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() /* && Wallclimb false */)
	{
		this->isWallRight = false;						// set the bool false so it only can on of them be true (safty first)
		this->isWallLeft = true;					
	}
}
/// Left Walldetector End
void AMyProjectCharacter::EndLeftWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() /* && Wallclimb false */)
	{
		this->isWallLeft = false;
		this->isWallRight = false;

	}
}


						//////////////////////////////////////
						//////////	   Timeline     //////////
						//////////////////////////////////////

///Wallrun
void AMyProjectCharacter::WallrunFloatReturn(float value)											// This Updates the Timeline
{
	if (this->isOnWall == true)
	{
		if (this->isWallRight == true)
		{

			FVector rayStartFindWall = this->GetActorTransform().GetLocation();
			FVector rayEndFindWall = this->GetActorTransform().GetRotation().GetRightVector();
			rayEndFindWall = rayEndFindWall * 150.0f;
			rayEndFindWall = rayEndFindWall + rayStartFindWall;

			FCollisionQueryParams rayParams = FCollisionQueryParams(FName(TEXT("WallRightParam")), true, this);		// Params for the RayCast
			rayParams.bTraceComplex = false;
			rayParams.bTraceAsyncScene = false;
			rayParams.bReturnPhysicalMaterial = false;
			rayParams.bIgnoreBlocks = false;

			FHitResult hitResult(ForceInit);

			bool hit = world->LineTraceSingleByChannel(			// Raycast
				hitResult,										// Result
				rayStartFindWall,								// RayStart
				rayEndFindWall,									// RayEnd
				ECC_Pawn,
				rayParams);
			
			if (hitResult.IsValidBlockingHit() == true)
			{
				if (hitResult.GetActor()->ActorHasTag("RunWall"))
				{
					FVector rayHitLocation = hitResult.Location;
					FVector wallForwardVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * 10;
					FVector wallBackwardsVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * -10;
					FVector cameraRotation = this->FirstPersonCameraComponent->GetComponentTransform().GetRotation().GetForwardVector() * 1000 + this->FirstPersonCameraComponent->GetComponentTransform().GetLocation();
					wallForwardVector = wallForwardVector + rayHitLocation;
					wallBackwardsVector = wallBackwardsVector + rayHitLocation;

					FVector forwardVector = cameraRotation - wallForwardVector;
					FVector backwardsVector = cameraRotation - wallBackwardsVector;

					if (forwardVector.Size() < backwardsVector.Size())
					{
						this->wallRunDirection = wallForwardVector;
						this->WallrunLaunch();
					}
					else
					{
						this->wallRunDirection = wallBackwardsVector;
						this->WallrunLaunch();
					}

				}
			}
		}
		else if (this->isWallLeft == true)
		{
			FVector rayStart = this->GetActorTransform().GetLocation();
			FVector rayEnd = this->GetActorTransform().GetRotation().GetRightVector();
			rayEnd = rayEnd * -150.0f;
			rayEnd = rayEnd + rayStart;

			FCollisionQueryParams rayParams = FCollisionQueryParams(FName(TEXT("WallRightParam")), true, this);		// Params for the RayCast
			rayParams.bTraceComplex = false;
			rayParams.bTraceAsyncScene = false;
			rayParams.bReturnPhysicalMaterial = false;
			rayParams.bIgnoreBlocks = false;

			FHitResult hitResult(ForceInit);

			world->LineTraceSingleByChannel(	// Raycast
				hitResult,						// Result
				rayStart,						// RayStart
				rayEnd,							// RayEnd
				ECC_Pawn,
				rayParams);

			if (hitResult.IsValidBlockingHit() == true)
			{
				if (hitResult.GetActor()->ActorHasTag("RunWall"))
				{
					FVector rayHitLocation = hitResult.Location;
					FVector wallForwardVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * 10;
					FVector wallBackwardsVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * -10;
					FVector cameraRotation = this->FirstPersonCameraComponent->GetComponentTransform().GetRotation().GetForwardVector() * 1000 + this->FirstPersonCameraComponent->GetComponentTransform().GetLocation();
					wallForwardVector = wallForwardVector + rayHitLocation;
					wallBackwardsVector = wallBackwardsVector + rayHitLocation;

					FVector forwardVector = cameraRotation - wallForwardVector;
					FVector backwardsVector = cameraRotation - wallBackwardsVector;

					if (forwardVector.Size() < backwardsVector.Size())
					{
						this->wallRunDirection = wallForwardVector;
						this->WallrunLaunch();
					}
					else
					{
						this->wallRunDirection = wallBackwardsVector;
						this->WallrunLaunch();
					}
				}
			}
		}
	}
}
