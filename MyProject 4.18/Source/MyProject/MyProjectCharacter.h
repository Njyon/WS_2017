// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
///Unreal Stuff
#include "Engine.h"
//#include "GameFramework/Character.h"
///Our Stuff
#include "MyProject/MyProjectProjectile.h"
#include "Kismet/KismetMathLibrary.h"
///GERENRATED HURENSOHN
#include "MyProjectCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

						//////////////////////////////////////
						//////////		BODY		//////////
						//////////////////////////////////////

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* FP_Gun;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* FP_Gun_1;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* FP_MuzzleLocationRight;
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* FP_MuzzleLocationLeft;

								//////// Timelines ////////
	UPROPERTY()
		class UTimelineComponent* wallrunTimeline;			// Wallrun main Timeline
	UPROPERTY()
		class UTimelineComponent* camTiltRightTimeline;		// Wallrun camTiltRight Timeline
	UPROPERTY()
		class UTimelineComponent* camTiltLeftTimeline;		// Wallrun camTiltLeft Timeline

	UPROPERTY()
		class UTimelineComponent* slideheightTimeline;		//slide height Timeline
	
	UPROPERTY()												//slide radius Timeline
		class UTimelineComponent* slideradiusTimeline;

								//////// Collision ////////
	UPROPERTY(EditAnywhere, Category = Detectors)
		class UBoxComponent* wallDetector;					// Wallrun main walldetector

	UPROPERTY(EditAnywhere, Category = Detectors)
		class UBoxComponent* wallRightDetector;				// Wallrun right Detector

	UPROPERTY(EditAnywhere, Category = Detectors)
		class UBoxComponent* wallLeftDetector;				// Wallrun left Detector


public:								////// PUBLIC //////
	AMyProjectCharacter();   // Konstructor

										//Variables//
	UPROPERTY(EditAnywhere, Category = Spawn)
	FVector spawnPoint;
	FRotator spawnRotation;
	FVector playerpos;
	
										// UPROPERTY //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool canJumpNow = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isSpacebarPressed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isFlying = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isOnLadder = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool godMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Sprint)
		bool canSprint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Slide)
		bool sliding = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		bool isShootingLeft = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ladder)
		bool isWallRight = false;                    // is on wall Right?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ladder)
		bool isWallLeft = false;                    // is on wall Left?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		int magazineSize = 12;
	UPROPERTY(BlueprintReadOnly, Category = Gun)
		int currentAmmo = magazineSize;

	//FireRate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		float fireRateSlomo = 0.05f;												//Set Fire Rate in Slomo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		float firerateNoSlomo = 0.3f;												//Set Fire Rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float gravitation = 2;														// Set Gravitation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float wallrunDuration = 1.5f;												// Set Wallrun Duration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float airControll = 0.2f;													// Set AirControll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float wallJumpForce = 500;													// Set the force to Jump from the wall
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float wallJumpForceForward = 1100;											// Set the force to Jump from the wall in View direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float jumpHeight = 1000;													// Set Jump Height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float jumpHeightOnWall = 600;												// Set Jump Height on Wall
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float jumpHeightOnWallUp = 1100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float climbEndBoost = -500;													// Set Climb End Boost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float maxRessource = 100;													// Set Max Ressources
	UPROPERTY(BlueprintReadOnly, Category = Ressources)
		float ressource;															// Set Ressources
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float ressourceFillAmmount = 20;											// Set FillAmount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float ressourceDrainAmount = 250;											// Set Drain Amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float sprintDrainAmount = 25;												// Set Drain Amount Sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float slideDrainAmount = 25;												// Set Drain Amount Sprint
	UPROPERTY(BlueprintReadOnly, Category = Getter)
		float HAxis;																// Horizontal Axis
	UPROPERTY(BlueprintReadOnly, Category = Getter)
		float VAxis;																// Vertical Axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		int sprintSpeed = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		int walkSpeed = 1100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crank)
		float notMoving = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float losingHealthTimer = 1;


	// is Slomo Active or Deactive
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slomo)
		bool isSlomoActive;

	// set Time Dilation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Slomo)
		float slomoTimeDilation = 0.05f;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float healthRechargeDelay = 1.0f;											// Healthrechargedelay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float healthPerDelay = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float lastTimeHitDelay = 1.0f;												// Healthrecharge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float crankDamage = 10.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float crankHealthThreshhold = 20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
		float hitAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeadTilt)
		FRotator normalCamRotation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimInstance* AnimInstance;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMyProjectProjectile> playerProjectile;
	/*UPROPERTY(EditDefaultsOnly, Category = Projectile)
		class UMyProjectProjectile* playerProjectile;*/

	//TimeLines
	UPROPERTY(EditAnywhere, Category = Timeline)
		class UCurveFloat* wallrunCurve;
	UPROPERTY(EditAnywhere, Category = Timeline)
		class UCurveFloat* tiltCamRightCurve;
	UPROPERTY(EditAnywhere, Category = Timeline)
		class UCurveFloat* tiltCamLeftCurve;

	UPROPERTY(EditAnywhere, Category = Timeline)
		class UCurveFloat* heightCurve;

	UPROPERTY(EditAnywhere, Category = Timeline)
		class UCurveFloat* radiusCurve;

										// Sounds //

	//Shoot
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* ShootAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* ShootAudioComponent;
	//Slide
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* SlideAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* SlideAudioComponent;
	//Wallrun
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* WallrunAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* WallrunAudioComponent;
	//Climb
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* ClimbAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* ClimbAudioComponent;
	//Slowmo
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* SlowmoAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* SlowmoAudioComponent;
	//SlowmoEnd
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* SlowmoEndAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* SlowmoEndAudioComponent;
	//Walk
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* WalkAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* WalkAudioComponent;
	//Jump
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* JumpAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* JumpAudioComponent;
	//PlayerHit
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* PlayerHitAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* PlayerHitAudioComponent;
	//Death
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* DeathAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* DeathAudioComponent;
	//Running
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* RunningAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* RunningAudioComponent;
	//Reload
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* ReloadAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* ReloadAudioComponent;

										// UFUNCTION //
	void Damage(int damage, FVector damageCauser);
	void SetRespawn(FVector spawnVector, FRotator spawnRotator);
	void Healthrecharge();
	void RessoourceRefill(float amount);

	/// Input
	UFUNCTION()
		void LMBPressed();	// Left Mouse Button Pressed
	UFUNCTION()
		void LMBReleased(); // Left Mouse Button Released
	UFUNCTION()
		void RMBPressed();	// Right Mouse Button Pressed
	UFUNCTION()
		void RMBReleased(); // Right Mouse Button Released
	UFUNCTION()
		void Jump();		// Spacebar Pressed
	UFUNCTION()
		void EndJumping();	// Spacebar Released
	UFUNCTION()
		void Slide();		// left Shift Pressed
	UFUNCTION()
		void EndSlide();	// left Shift Released
	UFUNCTION()
		void Sprint();		// left Shift Pressed
	UFUNCTION()
		void EndSprint();	// left Shift Released
	UFUNCTION()
		void Reload();

	virtual void Landed(const FHitResult& hit) override;						// Character touched the ground event

	UFUNCTION(BlueprintCallable)
		void Respawn();
	UFUNCTION(BlueprintCallable)
		void RespawnSound();
	UFUNCTION(BlueprintCallable)
		void SlideCam();
	UFUNCTION(BlueprintCallable)
		void RevertedSlideCam();
	UFUNCTION(BlueprintImplementableEvent)
		void OnDamageBPEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthRechargeBPEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnAmmoChange();
	UFUNCTION(BlueprintImplementableEvent)
		void OnResourceChange();
	UFUNCTION(BlueprintImplementableEvent)
		void FullStamina();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayDeathAnim();
	UFUNCTION(BlueprintImplementableEvent)
		void OnCanNotShootBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnIsDeadBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnRespawnBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnBulletSpawnBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWalkingBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnCrankDamageBpEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void OnClimbBPEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnClimbEndBPEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSlideBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlideEndBpEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void OnWallRunBpEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWallRunEndBpEvent();

	/// Timeline Floats
	UFUNCTION()
		void WallrunFloatReturn(float value);									// Updates the Wallrun Timeline
	UFUNCTION()
		void TiltCamRightFloatReturn(float value);								// Updates the Camera Tilt Right
	UFUNCTION()
		void TiltCamLeftFloatReturn(float value);								// Updates the Camera Tilt Left

	UFUNCTION()
		void SlideHeightFloatReturn(float height);

	UFUNCTION()
		void SlideRadiusFloatReturn(float radius);

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:								////// PRIVATE //////

											// UPROPERTY //
	///DataType
	int wallCollisionCounter = 0;	// Counts the Collision overlaps (Prevents bug)
	float soundTimeDilation;
	float helperWallJumpNegativeFloat = 0;
	bool isLMBPressed;
	bool isBulletFired = false;
	bool isShootingInNormalSpeed;				// Check you switched the Time Dilation
	bool isOnWall = false;						// is on wall?
	bool wallrunDoOnce = true;
	bool ishiftButtonPressed = false;
	bool isFalling = false;
	bool climbingSoundDoOnce = false;
	bool dead = false;
	bool isHit = false;
	bool onNotMoving = false;
	bool ismovingTimer = false;
	bool islosingHealth = false;
	bool WallrunUp = false;
	bool LadderDoOnce = false;
	bool gothitlessthan30 = false;
	
	///Struct
	FVector wallRunDirection;								// Helper for allrun
	FVector playerDirection;								// Helper for Wallrun
	FVector playerRightVector;								// Helper for Wallrun
	FRotator currentCamRotation;
	FRotator tiltedCamRotation;
	FTimerHandle timeHandle;								// needed for set Timer
	FTimerHandle wallrunHandle;								// Timehandle (Delay for the Wallrun)
	FTimerHandle delay;
	FTimerHandle healthrecharge;
	FTimerHandle respawn;
	FTimerHandle noMoving;
	FTimerHandle losingHealth;
	FBodyInstance* camRay;									// RayCast from Camera
	FVector acceleration;
	///Class
	class UCharacterMovementComponent* movementComponent; // Movement Component
	class UCapsuleComponent* capsuleComponent;
	class UWorld* world;	// Safe the world
	class AController* playerController;

											// UFUNCTION //

	void SpawnBullet();		// Spawns the bullet Blueprint
	void BulletCooldown();	// Sets the isBulletFired bool
	void WallrunLaunch();
	void GravitationOff();
	void GravitationOn();
	void WallrunRetriggerableDelay();
	void WallrunEnd();
	void WallrunEndUp();
	void GotHit();
	void NotMoving();
	void LosingHealth();
	

	///////////////////
	//// Collision ////
	///////////////////

	/// Main Wallrun detector Collision
	UFUNCTION()
	void OnWallDetected(
		class UPrimitiveComponent* hitComp,
		class AActor* otherActor,
		class UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool fromSweep, 
		const FHitResult & sweepResult
	);
	UFUNCTION()
	void EndWallDetected(
		class UPrimitiveComponent* hitComp,
		class AActor* otherActor,
		class UPrimitiveComponent* otherComp,
		int32 otherBodyIndex
	);

	/// Right Wallrun detector Collsision
	UFUNCTION()
		void OnRightWallDetected(
			class UPrimitiveComponent* hitComp,
			class AActor* otherActor,
			class UPrimitiveComponent* otherComp,
			int32 otherBodyIndex,
			bool fromSweep,
			const FHitResult & sweepResult
		);
	UFUNCTION()
		void EndRightWallDetected(
			class UPrimitiveComponent* hitComp,
			class AActor* otherActor,
			class UPrimitiveComponent* otherComp,
			int32 otherBodyIndex
		);

	/// Left Wallrun detector Collsision
	UFUNCTION()
		void OnLeftWallDetected(
			class UPrimitiveComponent* hitComp,
			class AActor* otherActor,
			class UPrimitiveComponent* otherComp,
			int32 otherBodyIndex,
			bool fromSweep,
			const FHitResult & sweepResult
		);
	UFUNCTION()
		void EndLeftWallDetected(
			class UPrimitiveComponent* hitComp,
			class AActor* otherActor,
			class UPrimitiveComponent* otherComp,
			int32 otherBodyIndex
		);


protected:								////// Protected //////
	virtual void BeginPlay(); //Executes at Begin+
	virtual void PostInitializeComponents() override; //Executes after Begin Play
	virtual void Tick(float DeltaSeconds) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;
	// End of APawn interface
	/** Handles moving forward/backward */
	UFUNCTION()
	void MoveForward(float val);

	/** Handles stafing movement, left and right */
	UFUNCTION()
	void MoveRight(float val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION()
	void TurnAtRate(float rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION()
	void LookUpAtRate(float rate);


	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
};

