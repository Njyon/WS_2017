// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
///Unreal Stuff
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
///Our Stuff
#include "MyProject/MyProjectProjectile.h"
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
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

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

										// UPROPERTY //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isSpacebarPressed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isFlying = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ladder)
		bool isOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine)
		int magazineSize;
	UPROPERTY(BlueprintReadOnly, Category = Magazine)
		int currentAmmo;
	//FireRate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FireRate)
		float fireRateSlomo = 0.05f;											//Set Fire Rate in Slomo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FireRate)
		float firerateNoSlomo = 0.3f;											//Set Fire Rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float gravitation;														// Set Gravitation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float wallrunDuration;													// Set Wallrun Duration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float airControll;														// Set AirControll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float wallJumpForce;													// Set the force to Jump from the wall
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float wallJumpForceForward;												// Set the force to Jump from the wall in View direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float jumpHeight;														// Set Jump Height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float jumpHeightOnWall;													// Set Jump Height on Wall
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GeneralMovementCPP)
		float climbEndBoost;													// Set Climb End Boost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float maxRessource;														// Set Max Ressources
	UPROPERTY(BlueprintReadOnly, Category = Ressources)
		float ressource;														// Set Ressources
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float ressourceFillAmmount;												// Set FillAmount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ressources)
		float ressourceDrainAmount;												// Set Drain Amount

	// is Slomo Active or Deactive
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slomo)
		bool isSlomoActive;

	// set Time Dilation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Slomo)
		float slomoTimeDilation = 0.05f;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Health = 100.0f;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
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

										// UFUNCTION //
	void Damage(int damage);

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
		void Slide();		// left Shift Preessed
	UFUNCTION()
		void EndSlide();	// left Shift Released
	UFUNCTION()
		void Reload();

	virtual void Landed(const FHitResult& hit) override;						// Character touched the ground event

	UFUNCTION(BlueprintImplementableEvent)
		void OnDamageBPEvent();

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
	int wallCollisionCounter = 0;							// Counts the Collision overlaps (Prevents bug)
	float soundTimeDilation;
	float HAxis;	// Horizontal Axis
	float VAxis;	// Vertical Axis
	float helperWallJumpNegativeFloat = 0;
	bool isLMBPressed;
	bool isBulletFired = false;
	bool isShootingInNormalSpeed;				// Check you switched the Time Dilation
	bool isOnWall = false;						// is on wall?
	bool isWallRight = false;					// is on wall Right?
	bool isWallLeft = false;					// is on wall Left?
	bool wallrunDoOnce = true;
	bool ishiftButtonPressed = false;
	bool sliding = false;
	bool isFalling = false;
	bool climbingSoundDoOnce = false;
	bool WallrunSoundDoOnce = false;
	
	///Struct
	FVector wallRunDirection;								// Helper for Wallrun
	FVector playerDirection;								// Helper for Wallrun
	FVector playerRightVector;								// Helper for Wallrun
	FRotator currentCamRotation;
	FRotator tiltedCamRotation;
	FRotator normalCamRotation;
	FTimerHandle timeHandle;								// needed for set Timer
	FTimerHandle wallrunHandle;								// Timehandle (Delay for the Wallrun)
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
	void WallrunRetriggerableDelay();
	void WallrunEnd();
	void SlideCam();
	void RevertedSlideCam();

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

