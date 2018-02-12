// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "TP_ThirdPersonCharacter.generated.h"

UCLASS(config=Game)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, Category = Detectors)
	//	class UBoxComponent* head;						// head

	//UPROPERTY(EditAnywhere, Category = Detectors)
	//	class UBoxComponent* upperBody;					//Upper Body

	////UPROPERTY(EditAnywhere, Category = Detectors)
	////	class UBoxComponent* armLeft;					// Left Arm

	////UPROPERTY(EditAnywhere, Category = Detectors)
	////	class UBoxComponent* armRight;					// Right Arm

	//UPROPERTY(EditAnywhere, Category = Detectors)
	//	class UBoxComponent* legLeft;					// Left Leg

	//UPROPERTY(EditAnywhere, Category = Detectors)
	//	class UBoxComponent* legRight;					// Right Leg

public:
	ATP_ThirdPersonCharacter();
	//DataType

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	UPROPERTY()
		FVector spawnpoint;
	UPROPERTY()
		FRotator spawnRotation;


	//Funcs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = health)
		float Health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = health)
		float MaxHealth = 100.0f;
	UFUNCTION(BlueprintCallable, Category = Gameplay)
		float GetCurrentHealth();
	UFUNCTION(BlueprintImplementableEvent)
		void OnNPCDeathBPEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void DamageEvent();
	UFUNCTION(Blueprintcallable)
		void Walking();
	UFUNCTION(Blueprintcallable)
		void StopWalking();
	UFUNCTION(Blueprintcallable)
		void Shooting();
	UFUNCTION()
		void EnemyRespawn();
	UFUNCTION()
		void SpawnAudio();

	void Damage(int damage);


								//Sound
	//Walk
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* WalkAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* WalkAudioComponent;

	//Shoot
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* ShootAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* ShootAudioComponent;

	//Death
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* DeathAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* DeathAudioComponent;

	//Spawn
	UPROPERTY(BlueprintReadOnly, Category = Audio)
		class USoundCue* SpawnAudioCue;
	UPROPERTY(EditAnywhere, meta = (BlueprintSpawnableComponent), BlueprintReadWrite, Category = Audio)
		class UAudioComponent* SpawnAudioComponent;

protected:

	virtual void BeginPlay(); //Executes at Begin+
	virtual void PostInitializeComponents() override; //Executes after Begin Play
	virtual void Tick(float DeltaSeconds) override;

//	/** Resets HMD orientation in VR. */
//	void OnResetVR();
//
//	/** Called for forwards/backward input */
//	void MoveForward(float Value);
//
//	/** Called for side to side input */
//	void MoveRight(float Value);
//
//	/** 
//	 * Called via input to turn at a given rate. 
//	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
//	 */
//	void TurnAtRate(float Rate);
//
//	/**
//	 * Called via input to turn look up/down at a given rate. 
//	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
//	 */
//	void LookUpAtRate(float Rate);
//
//	/** Handler for when a touch input begins. */
//	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
//
//	/** Handler for when a touch input stops. */
//	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
//
//protected:
//	// APawn interface
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//	// End of APawn interface

private:
	class UWorld* world;

	float soundTimeDilation;
};

