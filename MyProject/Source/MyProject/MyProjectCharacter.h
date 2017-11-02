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

	//FireRate
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FireRate)
		float fireRateSlomo = 0.05f;											//Set Fire Rate in Slomo
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FireRate)
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
	void LMBPressed();	// Left Mouse Button Pressed
	void LMBReleased(); // Left Mouse Button Released
	void RMBPressed();	// Right Mouse Button Pressed
	void RMBReleased(); // Right Mouse Button Released
	void Jump();		// Spacebar Pressed
	void EndJumping();	// Spacebar Released

	virtual void Landed(const FHitResult& hit) override;						// Character touched the ground event

	UFUNCTION(BlueprintImplementableEvent)
		void OnDamageBPEvent(float health);										// Event that gets called in Blueprint

	/// Timeline Floats
	UFUNCTION()
		void WallrunFloatReturn(float value);									// Updates the Wallrun Timeline
	UFUNCTION()
		void TiltCamRightFloatReturn(float value);								// Updates the Camera Tilt Right
	UFUNCTION()
		void TiltCamLeftFloatReturn(float value);								// Updates the Camera Tilt Left

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:								////// PRIVATE //////

											// UPROPERTY //
	///DataType
	int wallCollisionCounter = 0;							// Counts the Collision overlaps (Prevents bug)
	float Health = 100.0f;									// Character Health
	float soundTimeDilation;
	float HAxis;											// Horizontal Axis
	float VAxis;											// Vertical Axis
	float helperWallJumpNegativeFloat = 0;					// Helper Variable (WALLJUMP)
	bool isLMBPressed;										// Left Mouse Button Pressed?
	bool isBulletFired = false;								// Used for the FireRate
	bool isShootingInNormalSpeed;							// Check you switched the Time Dilation
	bool isOnWall = false;									// is on wall?
	bool isWallRight = false;								// is on wall Right?
	bool isWallLeft = false;								// is on wall Left?
	bool wallrunDoOnce = true;								// DoOnce bool
	
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
	///Class
	class UCharacterMovementComponent* movementComponent;	// Movement Component
	class UWorld* world;									// Safe the world
	class AController* playerController;

											// UFUNCTION //

	void SpawnBullet();										// Spawns the bullet Blueprint
	void BulletCooldown();									// Sets the isBulletFired bool
	void WallrunLaunch();									// The Wallrun (Launched when gravity is 0)
	void GravitationOff();									// Turns Gravity off for the Wallrun
	void WallrunRetriggerableDelay();						// Delay for the wallrun so you can only run a certain Time (Can get Retriggered by switching the wall)
	void WallrunEnd();										// Ends the Wallrun

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
	virtual void Tick(float deltaTime) override;

	/** Handles moving forward/backward */
	void MoveForward(float val);

	/** Handles stafing movement, left and right */
	void MoveRight(float val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
};

