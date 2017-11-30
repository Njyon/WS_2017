// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
//#include "Camera/CameraComponent.h"
//#include "Components/CapsuleComponent.h"
//#include "Components/InputComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/Controller.h"
#include "ConstructorHelpers.h"
//#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{

	world = GetWorld();
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	/*head = CreateDefaultSubobject<UBoxComponent>(TEXT("Head"));
	head->SetupAttachment(this->GetOwner()->Find("SkeletalMesh"), TEXT("Head"));

	upperBody = CreateDefaultSubobject<UBoxComponent>(TEXT("UpperBody"));
	upperBody->SetupAttachment(this->GetOwner()->FindFunction("SkeletalMesh"), TEXT("Body"));

	legLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftLeg"));
	legLeft->SetupAttachment(this->GetOwner()->FindFunction("SkeletalMesh"), TEXT("LLeg"));

	legRight = CreateDefaultSubobject<UBoxComponent>(TEXT("RightLeg"));
	legRight->SetupAttachment(this->GetOwner()->FindFunction("SkeletalMesh"), TEXT("RLeg"));*/


	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//Sounds
	//WalkSound
	static ConstructorHelpers::FObjectFinder<USoundCue> WalkCue(TEXT("'/Game/Sound/SFX/Movement/sfx_Walking'"));
	WalkAudioCue = WalkCue.Object;
	WalkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WalkAudioComp"));
	WalkAudioComponent->bAutoActivate = false;
	WalkAudioComponent->SetupAttachment(RootComponent);

	//ShootSound
	static ConstructorHelpers::FObjectFinder<USoundCue> ShootCue(TEXT("'/Game/Sound/SFX/Weapon/sfx_WeaponFire'"));
	ShootAudioCue = ShootCue.Object;
	ShootAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootAudioComp"));
	ShootAudioComponent->bAutoActivate = false;
	ShootAudioComponent->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

//void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//	// Set up gameplay key bindings
//	check(PlayerInputComponent);
//	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
//	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
//
//	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_ThirdPersonCharacter::MoveForward);
//	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_ThirdPersonCharacter::MoveRight);
//
//	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
//	// "turn" handles devices that provide an absolute delta, such as a mouse.
//	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
//	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
//	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_ThirdPersonCharacter::TurnAtRate);
//	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
//	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_ThirdPersonCharacter::LookUpAtRate);
//
//	// handle touch devices
//	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP_ThirdPersonCharacter::TouchStarted);
//	PlayerInputComponent->BindTouch(IE_Released, this, &ATP_ThirdPersonCharacter::TouchStopped);
//
//	// VR headset functionality
//	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATP_ThirdPersonCharacter::OnResetVR);
//}

void ATP_ThirdPersonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (WalkAudioCue->IsValidLowLevelFast())					//WalkSound
	{
		WalkAudioComponent->SetSound(WalkAudioCue);
	}

	if (ShootAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		ShootAudioComponent->SetSound(ShootAudioCue);
	}
}

void ATP_ThirdPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ATP_ThirdPersonCharacter::Walking()
{
	if (WalkAudioComponent->IsPlaying() == false)
	{
		FVector rayStart = this->GetActorLocation();
		FVector rayEnd = rayStart + this->GetActorUpVector() * -200;


		FCollisionQueryParams rayParams = FCollisionQueryParams("Detection", false, this);		// Params for the RayCast
		rayParams.bTraceComplex = false;
		rayParams.bTraceAsyncScene = true;
		rayParams.AddIgnoredActor(this);
		rayParams.bReturnPhysicalMaterial = true;

		FHitResult hitMat(ForceInit);
		GetWorld()->LineTraceSingleByChannel(hitMat, rayStart, rayEnd, ECC_MAX, rayParams);

		if (hitMat.IsValidBlockingHit() == true)
		{
			//UE_LOG(LogTemp, Warning, TEXT("what"));
			if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)
			{
				WalkAudioComponent->SetIntParameter(FName("sfx_WalkingMaterial"), 1);
			}

			else if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
			{
				WalkAudioComponent->SetIntParameter(FName("sfx_WalkingMaterial"), 0);
			}

			WalkAudioComponent->Play();
		}
	}
}

void ATP_ThirdPersonCharacter::StopWalking()
{
	WalkAudioComponent->Stop();
}

void ATP_ThirdPersonCharacter::Shooting()
{
	ShootAudioComponent->Play();
}

float  ATP_ThirdPersonCharacter::GetCurrentHealth()
{
	return Health;
}

void ATP_ThirdPersonCharacter::Damage(int damage)
{

	UE_LOG(LogTemp, Warning, TEXT("HIT"));
	DamageEvent();
	Health = Health - damage;

	if(Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("dead"));
		AController* controller = this->GetController();
		//controller->PawnPendingDestroy(this);
		SetActorEnableCollision(false);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OnNPCDeathBPEvent();
	}
}


//void ATP_ThirdPersonCharacter::OnResetVR()
//{
//	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
//}
//
//void ATP_ThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
//{
//		Jump();
//}
//
//void ATP_ThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
//{
//		StopJumping();
//}
//
//void ATP_ThirdPersonCharacter::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void ATP_ThirdPersonCharacter::LookUpAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
//}
//
//void ATP_ThirdPersonCharacter::MoveForward(float Value)
//{
//	if ((Controller != NULL) && (Value != 0.0f))
//	{
//		// find out which way is forward
//		const FRotator Rotation = Controller->GetControlRotation();
//		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//		// get forward vector
//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//		AddMovementInput(Direction, Value);
//	}
//}
//
//void ATP_ThirdPersonCharacter::MoveRight(float Value)
//{
//	if ( (Controller != NULL) && (Value != 0.0f) )
//	{
//		// find out which way is right
//		const FRotator Rotation = Controller->GetControlRotation();
//		const FRotator YawRotation(0, Rotation.Yaw, 0);
//	
//		// get right vector 
//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//		// add movement in that direction
//		AddMovementInput(Direction, Value);
//	}
//}
//