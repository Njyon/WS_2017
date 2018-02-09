// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

///Unreal Stuff
#include "MyProjectCharacter.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
//#include "Camera/CameraComponent.h"
//#include "Components/CapsuleComponent.h"
//#include "Components/InputComponent.h"
//#include "GameFramework/InputSettings.h"
//#include "Animation/AnimInstance.h"
//#include "Kismet/GameplayStatics.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "TimerManager.h"
//#include "PhysicsEngine/BodyInstance.h"
//#include "ConstructorHelpers.h"
//#include "Components/TimelineComponent.h"
//#include "UnrealMathUtility.h"
///Our Stuff

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	//static ConstructorHelpers::FClassFinder<UClass> projectile(TEXT("Class'/Game/Blueprints/Player/Behaviour/PlayerProjectile'"));
	//if (projectile.Class != NULL)
	//{
	//	//playerProjectile = (UClass*)projectile.Class;
	//	playerProjectile = projectile.Class;
	//}

	static ConstructorHelpers::FClassFinder<AMyProjectProjectile> projectile(TEXT("'/Game/Blueprints/Player/Behaviour/PlayerProjectile'"));
	if (projectile.Class != NULL)
	{
		//playerProjectile = (UClass*)projectile.Class;
		playerProjectile = projectile.Class;
	}
			//////////////////////////////////////
			//////////		Sounds		//////////
			//////////////////////////////////////

	//ShootSound
	static ConstructorHelpers::FObjectFinder<USoundCue> ShootCue(TEXT("'/Game/Sound/SFX/Weapon/sfx_WeaponFire'"));
	ShootAudioCue = ShootCue.Object;
	ShootAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootAudioComp"));
	ShootAudioComponent->bAutoActivate = false;
	ShootAudioComponent->SetupAttachment(RootComponent);

	//SlideSound
	static ConstructorHelpers::FObjectFinder<USoundCue> SlideCue(TEXT("'/Game/Sound/SFX/Movement/sfx_Sliding'"));
	SlideAudioCue = SlideCue.Object;
	SlideAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SlideAudioComp"));
	SlideAudioComponent->bAutoActivate = false;
	SlideAudioComponent->SetupAttachment(RootComponent);

	//WallrunSound
	static ConstructorHelpers::FObjectFinder<USoundCue> WallrunCue(TEXT("'/Game/Sound/SFX/Movement/sfx_WallRun'"));
	WallrunAudioCue = WallrunCue.Object;
	WallrunAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WallrunAudioComp"));
	WallrunAudioComponent->bAutoActivate = false;
	WallrunAudioComponent->SetupAttachment(RootComponent);

	//ClimbSound
	static ConstructorHelpers::FObjectFinder<USoundCue> ClimbCue(TEXT("'/Game/Sound/SFX/Movement/sfx_ClimbWall'"));
	ClimbAudioCue = ClimbCue.Object;
	ClimbAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ClimbAudioComp"));
	ClimbAudioComponent->bAutoActivate = false;
	ClimbAudioComponent->SetupAttachment(RootComponent);

	//SlowmoSound
	static ConstructorHelpers::FObjectFinder<USoundCue> SlowmoCue(TEXT("'/Game/Sound/SFX/other/sfx_Slow-Mo'"));
	SlowmoAudioCue = SlowmoCue.Object;
	SlowmoAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SlowmoAudioComp"));
	SlowmoAudioComponent->bAutoActivate = false;
	SlowmoAudioComponent->SetupAttachment(RootComponent);

	//WalkSound
	static ConstructorHelpers::FObjectFinder<USoundCue> WalkCue(TEXT("'/Game/Sound/SFX/Movement/sfx_Walking'"));
	WalkAudioCue = WalkCue.Object;
	WalkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WalkAudioComp"));
	WalkAudioComponent->bAutoActivate = false;
	WalkAudioComponent->SetupAttachment(RootComponent);

	//JumpSound
	static ConstructorHelpers::FObjectFinder<USoundCue> JumpCue(TEXT("'/Game/Sound/SFX/Movement/sfx_Jump'"));
	JumpAudioCue = JumpCue.Object;
	JumpAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpAudioComp"));
	JumpAudioComponent->bAutoActivate = false;
	JumpAudioComponent->SetupAttachment(RootComponent);

	//PlayerHitSound
	static ConstructorHelpers::FObjectFinder<USoundCue> HitCue(TEXT("'/Game/Sound/SFX/HIts/sfx_PlayerHit'"));
	PlayerHitAudioCue = HitCue.Object;
	PlayerHitAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioComp"));
	PlayerHitAudioComponent->bAutoActivate = false;
	PlayerHitAudioComponent->SetupAttachment(RootComponent);

	//DeathSound
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathCue(TEXT("'/Game/Sound/SFX/Character/Death/sfx_Death'"));
	DeathAudioCue = DeathCue.Object;
	DeathAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathAudioComp"));															
	DeathAudioComponent->bAutoActivate = false;
	DeathAudioComponent->SetupAttachment(RootComponent);

	//PlayerHitSound
	static ConstructorHelpers::FObjectFinder<USoundCue> SlowmoEndCue(TEXT("'/Game/Sound/SFX/other/sfx_SlowMoOut'"));
	SlowmoEndAudioCue = SlowmoEndCue.Object;
	SlowmoEndAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SlowmoEndAudioComp"));
	SlowmoEndAudioComponent->bAutoActivate = false;
	SlowmoEndAudioComponent->SetupAttachment(RootComponent);

	//RunningSound
	static ConstructorHelpers::FObjectFinder<USoundCue> RunningCue(TEXT("'/Game/Sound/SFX/Movement/sfx_Running'"));
	RunningAudioCue = RunningCue.Object;
	RunningAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("RunningAudioComp"));
	RunningAudioComponent->bAutoActivate = false;
	RunningAudioComponent->SetupAttachment(RootComponent);

	//ReloadSound
	static ConstructorHelpers::FObjectFinder<USoundCue> ReloadCue(TEXT("'/Game/Sound/SFX/Weapon/sfx_Reload'"));
	ReloadAudioCue = ReloadCue.Object;
	ReloadAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ReloadAudioComp"));
	ReloadAudioComponent->bAutoActivate = false;
	ReloadAudioComponent->SetupAttachment(RootComponent);

	//LandingSound
	static ConstructorHelpers::FObjectFinder<USoundCue> LandingCue(TEXT("'/Game/Sound/SFX/Movement/sfx_LandingCue'"));
	LandingAudioCue = LandingCue.Object;
	LandingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LandingAudioComp"));
	LandingAudioComponent->bAutoActivate = false;
	LandingAudioComponent->SetupAttachment(RootComponent);

	//VaultSound
	static ConstructorHelpers::FObjectFinder<USoundCue> VaultCue(TEXT("'/Game/Sound/SFX/Movement/sfx_Vault'"));
	VaultAudioCue = VaultCue.Object;
	VaultAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VaultAudioComp"));
	VaultAudioComponent->bAutoActivate = false;
	VaultAudioComponent->SetupAttachment(RootComponent);

	//Dialogue_Intro
	static ConstructorHelpers::FObjectFinder<USoundCue> Dialogue_IntroCue(TEXT("'/Game/Sound/SFX/Character/Dialogue/sfx_Intro'"));
	Dialogue_IntroAudioCue = Dialogue_IntroCue.Object;
	Dialogue_IntroAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Dialogue_IntroAudioComp"));
	Dialogue_IntroAudioComponent->bAutoActivate = false;
	Dialogue_IntroAudioComponent->SetupAttachment(RootComponent);

	//Dialogue_HalfWay
	static ConstructorHelpers::FObjectFinder<USoundCue> Dialogue_HalfWayCue(TEXT("'/Game/Sound/SFX/Character/Dialogue/sfx_halfwayThere'"));
	Dialogue_HalfWayAudioCue = Dialogue_HalfWayCue.Object;
	Dialogue_HalfWayAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Dialogue_HalfWayAudioComp"));
	Dialogue_HalfWayAudioComponent->bAutoActivate = false;
	Dialogue_HalfWayAudioComponent->SetupAttachment(RootComponent);

	//Dialogue_Almost
	static ConstructorHelpers::FObjectFinder<USoundCue> Dialogue_AlmostCue(TEXT("'/Game/Sound/SFX/Character/Dialogue/sfx_AlmostThere'"));
	Dialogue_AlmostAudioCue = Dialogue_AlmostCue.Object;
	Dialogue_AlmostAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Dialogue_AlmostAudioComp"));
	Dialogue_AlmostAudioComponent->bAutoActivate = false;
	Dialogue_AlmostAudioComponent->SetupAttachment(RootComponent);

	//Dialogue_Outro
	static ConstructorHelpers::FObjectFinder<USoundCue> Dialogue_OutroCue(TEXT("'/Game/Sound/SFX/Character/Dialogue/sfx_Outro'"));
	Dialogue_OutroAudioCue = Dialogue_OutroCue.Object;
	Dialogue_OutroAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Dialogue_OutroAudioComp"));
	Dialogue_OutroAudioComponent->bAutoActivate = false;
	Dialogue_OutroAudioComponent->SetupAttachment(RootComponent);

	//NormalHit
	static ConstructorHelpers::FObjectFinder<USoundCue> NormalHitCue(TEXT("'/Game/Sound/SFX/HIts/sfx_NormalHit'"));
	NormalHitAudioCue = NormalHitCue.Object;
	NormalHitAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NormalHitAudioComp"));
	NormalHitAudioComponent->bAutoActivate = false;
	NormalHitAudioComponent->SetupAttachment(RootComponent);

	//LandingCharSmall
	static ConstructorHelpers::FObjectFinder<USoundCue> LandingCharSmallHitCue(TEXT("'/Game/Sound/SFX/Movement/sfx_LandingCharSmall'"));
	LandingCharSmallAudioCue = LandingCharSmallHitCue.Object;
	LandingCharSmallAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LandingCharSmallAudioComp"));
	LandingCharSmallAudioComponent->bAutoActivate = false;
	LandingCharSmallAudioComponent->SetupAttachment(RootComponent);

	//LandingCharLarge
	static ConstructorHelpers::FObjectFinder<USoundCue> LandingCharLargeCue(TEXT("'/Game/Sound/SFX/Movement/sfx_LandingCharLarge'"));
	LandingCharLargeAudioCue = LandingCharLargeCue.Object;
	LandingCharLargeAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LandingCharLargeAudioComp"));
	LandingCharLargeAudioComponent->bAutoActivate = false;
	LandingCharLargeAudioComponent->SetupAttachment(RootComponent);


	
			////////////End Sounds////////////////

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 0.45f;
	BaseLookUpRate = 0.45f;

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
	Mesh1P->RelativeRotation = FRotator(-12.0, 2.0f, -90.0f);
	Mesh1P->RelativeLocation = FVector(42.0f, -5.0f, -163.0f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_Gun_1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun_l"));
	FP_Gun_1->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun_1->bCastDynamicShadow = false;
	FP_Gun_1->CastShadow = false;
	FP_Gun_1->SetupAttachment(Mesh1P, TEXT("GripPoint_l"));
	FP_Gun_1->SetupAttachment(RootComponent);

	FP_MuzzleLocationRight = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationRight"));
	FP_MuzzleLocationRight->SetupAttachment(FP_Gun);
	FP_MuzzleLocationRight->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FP_MuzzleLocationRight->AttachTo(FP_Gun, "Muzzle");

	FP_MuzzleLocationLeft = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationLeft"));
	FP_MuzzleLocationLeft->SetupAttachment(FP_Gun_1);
	FP_MuzzleLocationLeft->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FP_MuzzleLocationLeft->AttachTo(FP_Gun_1, "Muzzle");

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	movementComponent = GetCharacterMovement();					// Get the CharacterMovement Component
	capsuleComponent = GetCapsuleComponent();
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
	static ConstructorHelpers::FObjectFinder<UCurveFloat> WallCurve(TEXT("'/Game/Blueprints/Curves/WallRun'"));
	wallrunCurve = WallCurve.Object;
	wallrunTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WallrunTimeline"));

	// Slide Timeline
	static ConstructorHelpers::FObjectFinder<UCurveFloat> HeightCurve(TEXT("'/Game/Blueprints/Curves/heightCurve'"));
	heightCurve = HeightCurve.Object;
	slideheightTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("slideheightTimeline"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> RadiusCurve(TEXT("'/Game/Blueprints/Curves/radiusCurve'"));
	radiusCurve = RadiusCurve.Object;
	slideradiusTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("slideradiusTimeline"));

	// TiltCam Right Timeline
	static ConstructorHelpers::FObjectFinder<UCurveFloat> TiltRightCurve(TEXT("'/Game/Blueprints/Curves/TiltCamRight'"));
	tiltCamRightCurve = TiltRightCurve.Object;
	camTiltRightTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CamTiltRightTimeline"));

	// TiltCam Left Timeline
	static ConstructorHelpers::FObjectFinder<UCurveFloat> TiltLeftCurve(TEXT("'/Game/Blueprints/Curves/TiltCamLeft'"));
	tiltCamLeftCurve = TiltLeftCurve.Object;
	camTiltLeftTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CamTiltLeftTimeline"));
}

			//////////////////////////////////////
			//////////	  Begin Play	//////////
			//////////////////////////////////////

void AMyProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	spawnPoint = GetActorLocation();
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FP_Gun_1->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_l"));
	
	this->movementComponent->SetPlaneConstraintEnabled(true);								// Enable PlaneContraint (Used for the wallrun)
	playerController = UGameplayStatics::GetPlayerController(world, 0);						// GetPlayerController

	
	// Declare Delegate function to be binded with WallrunFloatReturn(float value)
	FOnTimelineFloat WallrunUpdate;
	WallrunUpdate.BindUFunction(this, FName("WallrunFloatReturn"));
	// Declare Delegate function to be binded with TiltCamRightFloatReturn(float value)
	FOnTimelineFloat camTiltRightUpdate;
	camTiltRightUpdate.BindUFunction(this, FName("TiltCamRightFloatReturn"));
	// Declare Delegate function to be binded with TiltCamLeftFloatReturn(float value)
	FOnTimelineFloat camTiltLeftUpdate;
	camTiltLeftUpdate.BindUFunction(this, FName("TiltCamLeftFloatReturn"));

	// Declare Delegate function to be binded with SlideHeightFloatReturn(float value)
	FOnTimelineFloat SlideHeightUpdate;
	SlideHeightUpdate.BindUFunction(this, FName("SlideHeightFloatReturn"));

	// Declare Delegate function to be binded with SlideRadiusFloatReturn(float value)
	FOnTimelineFloat SlideRadiusUpdate;
	SlideRadiusUpdate.BindUFunction(this, FName("SlideRadiusFloatReturn"));

	// Check if Curve asset it valid
	if (this->wallrunCurve)
	{
		wallrunTimeline->AddInterpFloat(this->wallrunCurve, WallrunUpdate, FName("wallrunTimeline"));		// Set the Curve to the Delegate

		wallrunTimeline->SetLooping(true);
		wallrunTimeline->SetIgnoreTimeDilation(false);
	}
	// Check if Curve asset isValid
	if (this->tiltCamRightCurve)
	{
		camTiltRightTimeline->AddInterpFloat(this->tiltCamRightCurve, camTiltRightUpdate, FName("camTiltRightTimeline"));		// Set the Curve to the Delegate

		camTiltRightTimeline->SetIgnoreTimeDilation(true);					// Ignores TimeDelation (TEST)
		camTiltRightTimeline->SetPlayRate(5);
	}
	// Check if Curve asset isValid
	if (this->tiltCamLeftCurve)
	{
		camTiltLeftTimeline->AddInterpFloat(this->tiltCamLeftCurve, camTiltLeftUpdate, FName("camTiltLeftTimeline"));			// Set the Curve to the Delegate

		camTiltLeftTimeline->SetIgnoreTimeDilation(true);					// Ignores TimeDelation (TEST)
		camTiltLeftTimeline->SetPlayRate(5);
	}

	if (this->heightCurve)
	{
		slideheightTimeline->AddInterpFloat(this->heightCurve, SlideHeightUpdate, FName("slideheightTimeline"));
		
		slideheightTimeline->SetLooping(false);
		slideheightTimeline->SetIgnoreTimeDilation(true);
	}

	if (this->radiusCurve)
	{
		slideradiusTimeline->AddInterpFloat(this->radiusCurve, SlideRadiusUpdate, FName("slideradiusTimeline"));
		
		slideradiusTimeline->SetLooping(false);
		slideradiusTimeline->SetIgnoreTimeDilation(true);
	}

	this->helperWallJumpNegativeFloat = this->wallJumpForce * 2;
	this->helperWallJumpNegativeFloat = this->wallJumpForce - this->helperWallJumpNegativeFloat;

	this->movementComponent->MaxWalkSpeed = this->walkSpeed;

	this->ressource = this->maxRessource;
	//this->currentAmmo = this->magazineSize;
}

			///////////////////////////////////////
			/////////// Post Processing  //////////
			///////////////////////////////////////

void AMyProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ShootAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		ShootAudioComponent->SetSound(ShootAudioCue);
	}

	if (SlideAudioCue->IsValidLowLevelFast())					//SlideSound
	{	
		SlideAudioComponent->SetSound(SlideAudioCue);
	}

	if (WallrunAudioCue->IsValidLowLevelFast())					//WallrunSound
	{	
		WallrunAudioComponent->SetSound(WallrunAudioCue);
	}

	if (ClimbAudioCue->IsValidLowLevelFast())					//ClimbSound
	{	
		ClimbAudioComponent->SetSound(ClimbAudioCue);
	}

	if (SlowmoAudioCue->IsValidLowLevelFast())					//SlowmoSound
	{	
		SlowmoAudioComponent->SetSound(SlowmoAudioCue);
	}
	if (WalkAudioCue->IsValidLowLevelFast())					//WalkSound
	{
		WalkAudioComponent->SetSound(WalkAudioCue);
	}
	if (JumpAudioCue->IsValidLowLevelFast())					//JumpSound
	{	
		JumpAudioComponent->SetSound(JumpAudioCue);
	}
	if (PlayerHitAudioCue->IsValidLowLevelFast())                    //HitSound
	{
		PlayerHitAudioComponent->SetSound(PlayerHitAudioCue);
	}
	if (DeathAudioCue->IsValidLowLevelFast())                    //DeathSound
	{
		DeathAudioComponent->SetSound(DeathAudioCue);														
	}
	if (SlowmoEndAudioCue->IsValidLowLevelFast())                    //SlowmoEndSound
	{
		SlowmoEndAudioComponent->SetSound(SlowmoEndAudioCue);
	}
	if (RunningAudioCue->IsValidLowLevelFast())                    //RunningSound
	{
		RunningAudioComponent->SetSound(RunningAudioCue);
	}
	if (ReloadAudioCue->IsValidLowLevelFast())                    //ReloadSound
	{
		ReloadAudioComponent->SetSound(ReloadAudioCue);
	}
	if (LandingAudioCue->IsValidLowLevelFast())                    //LandingSound
	{
		LandingAudioComponent->SetSound(LandingAudioCue);
	}
	if (VaultAudioCue->IsValidLowLevelFast())                    //LandingSound
	{
		VaultAudioComponent->SetSound(VaultAudioCue);
	}
	if (Dialogue_IntroAudioCue->IsValidLowLevelFast())					//Dialogue_1
	{
		Dialogue_IntroAudioComponent->SetSound(Dialogue_IntroAudioCue);
	}
	if (Dialogue_HalfWayAudioCue->IsValidLowLevelFast())					//Dialogue_2
	{
		Dialogue_HalfWayAudioComponent->SetSound(Dialogue_HalfWayAudioCue);
	}
	if (Dialogue_AlmostAudioCue->IsValidLowLevelFast())					//Dialogue_3
	{
		Dialogue_AlmostAudioComponent->SetSound(Dialogue_AlmostAudioCue);
	}
	if (Dialogue_OutroAudioCue->IsValidLowLevelFast())					//Dialogue_4
	{
		Dialogue_OutroAudioComponent->SetSound(Dialogue_OutroAudioCue);
	}
	if (NormalHitAudioCue->IsValidLowLevelFast())					//NormalHit
	{
		NormalHitAudioComponent->SetSound(NormalHitAudioCue);
	}
	if (LandingCharSmallAudioCue->IsValidLowLevelFast())					//landingSmall
	{
		LandingCharSmallAudioComponent->SetSound(LandingCharSmallAudioCue);
	}
	if (LandingCharLargeAudioCue->IsValidLowLevelFast())					//landingLarge
	{
		LandingCharLargeAudioComponent->SetSound(LandingCharLargeAudioCue);
	}
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
	/// Input Left Mouse Button
	playerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProjectCharacter::LMBPressed);
	playerInputComponent->BindAction("Fire", IE_Released, this, &AMyProjectCharacter::LMBReleased);
	/// Input Right Mouse Button
	playerInputComponent->BindAction("Slomo", IE_Pressed, this, &AMyProjectCharacter::RMBPressed);
	playerInputComponent->BindAction("Slomo", IE_Released, this, &AMyProjectCharacter::RMBReleased);
	///	Input left Shift
	playerInputComponent->BindAction("Slide", IE_Pressed, this, &AMyProjectCharacter::Slide);
	playerInputComponent->BindAction("Slide", IE_Released, this, &AMyProjectCharacter::EndSlide);
	///Sprint
	playerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyProjectCharacter::Sprint);
	playerInputComponent->BindAction("Sprint", IE_Released, this, &AMyProjectCharacter::EndSprint);
	/// Reload
	playerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyProjectCharacter::Reload);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	/// Input WASD
	playerInputComponent->BindAxis("MoveForwardKey", this, &AMyProjectCharacter::MoveForward);
	playerInputComponent->BindAxis("StrafeKey", this, &AMyProjectCharacter::MoveRight);
	//playerInputComponent->BindAxis("Turn", this, &APawn::Turn);
	playerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	playerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);
	//playerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);
}

			//////////////////////////////////////
			//////////		TICK		//////////
			//////////////////////////////////////

void AMyProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

			//////////////////////////////////////
			//////////	Breathing Sound	//////////
			//////////////////////////////////////

	if (this->movementComponent->IsFalling())
	{
		if (this->GetVelocity().Equals(FVector(0, 0, 0), 0.000100f) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("falling speed: %s"), *this->fallingSpeed.ToString());
			fallingSpeed = this->GetVelocity();
		}
	}

	if (this->movementComponent->GetCurrentAcceleration().Equals(FVector(0, 0, 0), 0.000100f) == false)
	{
		if (!isBreathing)
		{
			breathingDelay = FMath::RandRange(2.0f, 6.0f);
			isBreathing = true;
			world->GetTimerManager().SetTimer(breathing, this, &AMyProjectCharacter::RunningAudioBegin, breathingDelay, false);
		}
	}

	if (this->movementComponent->GetCurrentAcceleration().Equals(FVector(0, 0, 0), 0.000100f) == true)
	{
		if (isBreathing)
		{
			isBreathing = false;
			RunningAudioEnd();
		}
	}

			//////////////////////////////////////
			//////////	  Rest Tick 	//////////
			//////////////////////////////////////


	if (isOnLadder == true)
	{
		if (LadderDoOnce == false)
		{
			OnClimbBPEvent();
		}
	}
	if (isOnLadder == false)
	{
		LadderDoOnce = false;
		OnClimbEndBPEvent();
	}

	if (this->sliding)
	{
		shootingState = ShootState::LeftShooting;
	}

	if (this->movementComponent->GetCurrentAcceleration().Equals(FVector(0, 0, 0), 0.000100f) && sliding == false && isOnWall == false && isOnLadder == false && this->movementComponent->IsMovingOnGround() == true)
	{
		if (!ismovingTimer)
		{
			//UE_LOG(LogTemp, Warning, TEXT("notmoving"));
			ismovingTimer = true;
			onNotMoving = true;
			world->GetTimerManager().SetTimer(noMoving, this, &AMyProjectCharacter::NotMoving, notMoving, false);
		}
	}
	if (this->movementComponent->GetCurrentAcceleration().Equals(FVector(0, 0, 0), 0.000100f) == false || sliding == true || isOnWall == true || isOnLadder == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("moving"));
		onNotMoving = false;
		ismovingTimer = false;
		islosingHealth = false;
		world->GetTimerManager().PauseTimer(losingHealth);
		world->GetTimerManager().ClearTimer(losingHealth);
		world->GetTimerManager().PauseTimer(noMoving);
		world->GetTimerManager().ClearTimer(noMoving);
	}
	if (islosingHealth == true && sliding == false && isOnWall == false && isOnLadder == false && this->movementComponent->IsMovingOnGround() == true)
	{
		if (world->GetTimerManager().IsTimerActive(losingHealth) == false) // Trigger Only Once and than wait for Cooldown (Cooldwon is Rewriteble)
		{
			world->GetTimerManager().SetTimer(losingHealth, this, &AMyProjectCharacter::LosingHealth, losingHealthTimer, false);
		}
	}
	

	///Sounds Slowmo
	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	ShootAudioComponent->SetFloatParameter(FName("sfx_WeaponFireSlowmo"), soundTimeDilation);				//ShootSound		
	SlideAudioComponent->SetFloatParameter(FName("sfx_SlidingSlowmo"), soundTimeDilation);				//SlideSound		
	WallrunAudioComponent->SetFloatParameter(FName("sfx_WallrunSlowmo"), soundTimeDilation);				//WallrunSound		
	ClimbAudioComponent->SetFloatParameter(FName("sfx_ClimbWallSlowmo"), soundTimeDilation);			//ClimbSound
	WalkAudioComponent->SetFloatParameter(FName("sfx_WalkingSlowmo"), soundTimeDilation);			//WalkSound
	JumpAudioComponent->SetFloatParameter(FName("sfx_JumpSlowmo"), soundTimeDilation);			//JumpSound
	RunningAudioComponent->SetFloatParameter(FName("sfx_RunningSlowmo"), soundTimeDilation);			//JumpSound
	ReloadAudioComponent->SetFloatParameter(FName("sfx_ReloadSlowmo"), soundTimeDilation);			//JumpSound
	NormalHitAudioComponent->SetFloatParameter(FName("sfx_NormalHitSlowmo"), soundTimeDilation);			//JumpSound
	LandingCharSmallAudioComponent->SetFloatParameter(FName("sfx_SmallLandingSlowmo"), soundTimeDilation);			//JumpSound
	LandingCharLargeAudioComponent->SetFloatParameter(FName("sfx_LargeLandingSlowmo"), soundTimeDilation);			//JumpSound


	if (isOnLadder == true && climbingSoundDoOnce == false)													//climbSound gets played and stopped
	{
		climbingSoundDoOnce = true;
		ClimbAudioComponent->Play();
	}
	if (isOnLadder == false && climbingSoundDoOnce == true)
	{
		ClimbAudioComponent->Stop();
		climbingSoundDoOnce = false;
	}

	if (isOnWall == true && WallrunAudioComponent->IsPlaying() == false)													//wallrunSound gets played and stopped
	{
		//UE_LOG(LogTemp, Warning, TEXT("1"));
		//WallrunSoundDoOnce = true;
		WallrunAudioComponent->Play();
	}
	if (isOnWall == false)
	{
		//UE_LOG(LogTemp, Warning, TEXT("2"));
		WallrunAudioComponent->Stop();

	}
	///Shoot
	if (isLMBPressed == true)
	{
		if (this->isBulletFired == false && this->canShoot == true)
		{
			this->isBulletFired = true;

			if (this->currentAmmo > 0)
			{
				SpawnBullet();		// Shoot a Bullet
				currentAmmo -= 1;
				OnAmmoChange();
			}
			else
			{
				Reload();
			}
		}

		else if (this->isBulletFired == true || this->canShoot == false)
		{
			OnCanNotShootBpEvent();
		}
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


	///Slomo
	if (isSlomoActive == true)
	{
		this->ressource -= world->GetDeltaSeconds() * this->ressourceDrainAmount;
		OnResourceChange();

		//if (movementComponent->IsFalling() == false && this->sliding == false)  // check if Character is in "Action"
		//{
		//	isSlomoActive = false;
		//	UGameplayStatics::SetGlobalTimeDilation(world, 1); // Set Time Dilation to Normal
		//	SlowmoAudioComponent->Stop();
		//}
		/*else*/ if (this->ressource < 0)
		{
			isSlomoActive = false;
			UGameplayStatics::SetGlobalTimeDilation(world, 1); // Set Time Dilation to Normal
			SlowmoAudioComponent->Stop();
			EndSprint();
			OnSlomoOff();
		}
	}

	if (canSprint)
	{
		this->ressource -= world->GetDeltaSeconds() * this->sprintDrainAmount;
		OnResourceChange();
	}

	if (sliding)
	{
		this->ressource -= world->GetDeltaSeconds() * this->sprintDrainAmount;
		OnResourceChange();
		onNotMoving = false;
	}

	//Ressource
	if (this->ressource < 100)
	{
		this->ressource += world->GetDeltaSeconds() * this->ressourceFillAmmount;
		OnResourceChange();
	}
	if (this->ressource > 100)
	{
		ressource = 100;
		FullStamina();
	}

	if (this->ressource <= 0)
	{
		EndSprint();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Ressources at : %f %"), this->ressource);

	//UE_LOG(LogTemp, Warning, TEXT("Ammo  : %i %"), this->currentAmmo);
	if (Health <= MaxHealth)
	{
		if (!isHit && !onNotMoving)
		{
			if (world->GetTimerManager().IsTimerActive(healthrecharge) == false)
			{
				world->GetTimerManager().SetTimer(healthrecharge, this, &AMyProjectCharacter::Healthrecharge, this->healthRechargeDelay, false);
			}
		}
	}
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
		OnCrankDamageBpEvent();
	}
	if (Health > 30.0f && gothitlessthan30 == true)
	{
		gothitlessthan30 = false;
	}
}

			//////////////////////////////////////
			//////////////////////////////////////
			//////////////////////////////////////



			//////////////////////////////////////
			//////////	    Input       //////////
			//////////////////////////////////////

							// Mouse //

void AMyProjectCharacter::TurnAtRate(float rate)
{
	if (this->blockInput == false)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(rate * BaseTurnRate /** GetWorld()->GetDeltaSeconds()*/);
	}
}

void AMyProjectCharacter::LookUpAtRate(float rate)
{
	if (this->blockInput == false)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(rate * BaseLookUpRate /** GetWorld()->GetDeltaSeconds()*/);
	}
}

void AMyProjectCharacter::LMBPressed()
{
	if (this->blockInput == false)
	{
		isLMBPressed = true;
	}
}

void AMyProjectCharacter::LMBReleased()
{
	if (this->blockInput == false)
	{
		isLMBPressed = false;
	}
}

void AMyProjectCharacter::RMBPressed()
{
	if (this->blockInput == false)
	{
		//if (movementComponent->IsFalling() == true || this->sliding == true) // Check if you can Set Slomo to Active
		//{
		if (this->ressource > 0)
		{
			isSlomoActive = true;
			UGameplayStatics::SetGlobalTimeDilation(world, slomoTimeDilation); // Set Time to Slomo Time Dilation
			OnSlomoOn();
			SlowmoAudioComponent->Play();
			SlowmoEndAudioComponent->Stop();
		}
	}
	/*}*/
}

void AMyProjectCharacter::RMBReleased()
{
	if (this->blockInput == false)
	{
		isSlomoActive = false;
		UGameplayStatics::SetGlobalTimeDilation(world, 1); // Set Time to Noraml
		OnSlomoOff();
		SlowmoAudioComponent->Stop();
		SlowmoEndAudioComponent->Play();
	}
}

						// Keyboard WASD //

void AMyProjectCharacter::MoveForward(float value)
{
	if (this->blockInput == false)
	{
		if (value != 0.0f && isOnLadder == true)
		{
			if (this->isFlying == false)
			{
				this->isFlying = true;
				this->movementComponent->SetMovementMode(EMovementMode::MOVE_Flying, 0);
			}

			AddMovementInput(GetActorUpVector(), value);
			this->VAxis = value;
		}
		else if (value != 0.0f && this->isOnWall == false && this->sliding == false && this->isOnLadder == false)
		{
			/*if (RunningAudioComponent->IsPlaying() == false)
			{
				RunningAudioComponent->FadeIn(3, 1, 0);
			}*/
			OnWalkingBpEvent();

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
				world->LineTraceSingleByChannel(hitMat, rayStart, rayEnd, ECC_MAX, rayParams);

				if (hitMat.IsValidBlockingHit() == true)
				{
					if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)
					{
							WalkAudioComponent->SetIntParameter(FName("sfx_WalkingMaterial"), 1);
					}

					else if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
					{
							WalkAudioComponent->SetIntParameter(FName("sfx_WalkingMaterial"), 0);
					}
					OnWalkingBpEvent();
					WalkAudioComponent->Play();
				}

				else
				{
					WalkAudioComponent->Stop();
				}
			}
			// add movement in that direction
			AddMovementInput(GetActorForwardVector(), value);
			this->VAxis = value;
		}
	}
}

void AMyProjectCharacter::MoveRight(float value)
{
	if (this->blockInput == false)
	{
		if (value != 0.0f && this->isOnWall == false && this->sliding == false && this->isOnLadder == false)
		{
			if (WalkAudioComponent->IsPlaying() == false)
			{
				OnWalkingBpEvent();
				FVector rayStart = this->GetActorLocation();
				FVector rayEnd = rayStart + this->GetActorUpVector() * -200;


				FCollisionQueryParams rayParams = FCollisionQueryParams("Detection", false, this);		// Params for the RayCast
				rayParams.bTraceComplex = false;
				rayParams.bTraceAsyncScene = true;
				rayParams.bReturnPhysicalMaterial = true;

				FHitResult hitMat(ForceInit);

				if (world->LineTraceSingleByChannel(hitMat, rayStart, rayEnd, ECC_Pawn, rayParams))
				{
					//UE_LOG(LogTemp, Warning, TEXT("%f"), hitMat.PhysMaterial);

					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "I see a Normal: " + hitMat);

					if (hitMat.IsValidBlockingHit() == true)
					{
						if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)
						{
								WalkAudioComponent->SetIntParameter(FName("sfx_WalkingMaterial"), 1);
						}

						else if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
						{
								WalkAudioComponent->SetIntParameter(FName("sfx_WalkingMaterial"), 0);
						}
						OnWalkingBpEvent();
						WalkAudioComponent->Play();
					}

					else
					{
						WalkAudioComponent->Stop();
					}
				}
			}
			// add movement in that direction
			AddMovementInput(GetActorRightVector(), value);
			this->HAxis = value;
		}
	}
}

void AMyProjectCharacter::Reload()
{
	if (this->blockInput == false)
	{
		if (this->currentAmmo < this->magazineSize)
		{
			ReloadAudioComponent->Play();
			isReloading = true;
			this->currentAmmo = this->magazineSize;
			OnAmmoChange();
		}
	}
}

						// Keyboard SpaceBar //

							//// Jump ////

void AMyProjectCharacter::Jump()
{
	if (this->blockInput == false)
	{
		this->isSpacebarPressed = true;
		if (this->movementComponent->IsMovingOnGround() == true)
		{
			this->movementComponent->GravityScale = gravitation;	// Set Gravity

			this->LaunchCharacter(									// Launch Character (Jump Up)
				FVector(0.0f, 0.0f, this->jumpHeight),				// Where to Launch
				false,												// XY Override
				true												// Z Override
			);

			//JumpSound

			if (JumpAudioComponent->IsPlaying() == false)
			{
				FVector rayStart = this->GetActorLocation();
				FVector rayEnd = rayStart + this->GetActorUpVector() * -500;


				FCollisionQueryParams rayParams = FCollisionQueryParams("Detection", false, this);		// Params for the RayCast
				rayParams.bTraceComplex = false;
				rayParams.bTraceAsyncScene = true;
				rayParams.AddIgnoredActor(this);
				rayParams.bReturnPhysicalMaterial = true;

				FHitResult hitMat(ForceInit);
				world->LineTraceSingleByChannel(hitMat, rayStart, rayEnd, ECC_MAX, rayParams);

				if (hitMat.IsValidBlockingHit() == true)
				{
					if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)
					{
						JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 1);
					}

					else if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
					{
						JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 0);
					}
					JumpAudioComponent->Play();
				}
			}
		}
		else if (this->isWallRight == true)
		{
			if (this->WallrunUp == true)
			{
				WallrunEndUp();
			}
			else
			{
				WallrunEnd();
			}

		}
		else if (this->isWallLeft == true)
		{
			if (this->WallrunUp == true)
			{
				WallrunEndUp();
			}

			else
			{
				WallrunEnd();
			}

		}
		else if (this->isOnLadder == true)
		{

			this->movementComponent->SetMovementMode(EMovementMode::MOVE_Flying, 0);
			this->isOnLadder = false;
			FVector launchVector = this->GetActorForwardVector() * this->climbEndBoost;

			this->LaunchCharacter(FVector(
				launchVector.X,
				launchVector.Y,
				this->jumpHeight),
				true,
				true
			);

			JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 3);
			JumpAudioComponent->Play();
		}
	}
}


void AMyProjectCharacter::EndJumping()
{
	this->isSpacebarPressed = false;
	this->StopJumping();					// Unreal Function
}

void AMyProjectCharacter::Landed(const FHitResult& hit) 
{
	//Super::Landed(hit);

	UE_LOG(LogTemp, Warning, TEXT("falling speed: %s"), *this->fallingSpeed.ToString());
	this->MakeNoise(				// Make Noise for AI Perception
		1.0f,						// Loudness
		this,						// Instigator (PAWN)
		hit.Location,				// Noise Location
		0.0f						// Max Range
	);

	if (fallingSpeed.Z > -2000)
	{
		if (!isOnLadder && !isOnWall)
		{
			LandingCharSmallAudioComponent->Play();
		}
	}

	if (fallingSpeed.Z <= -2000)
	{
		if (!isOnLadder && !isOnWall)
		{
			LandingCharLargeAudioComponent->Play();
		}
	}
	

	if (LandingAudioComponent->IsPlaying() == false)
	{
		FVector rayStart = this->GetActorLocation();
		FVector rayEnd = rayStart + this->GetActorUpVector() * -200;


		FCollisionQueryParams rayParams = FCollisionQueryParams("Detection", false, this);		// Params for the RayCast
		rayParams.bTraceComplex = false;
		rayParams.bTraceAsyncScene = true;
		rayParams.AddIgnoredActor(this);
		rayParams.bReturnPhysicalMaterial = true;

		FHitResult hitMat(ForceInit);
		world->LineTraceSingleByChannel(hitMat, rayStart, rayEnd, ECC_MAX, rayParams);

		if (hitMat.IsValidBlockingHit() == true)
		{
			if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType2)
			{
					LandingAudioComponent->SetIntParameter(FName("sfx_LandingMaterial"), 1);
			}

			else if (hitMat.PhysMaterial->SurfaceType.GetValue() == SurfaceType1)
			{
					LandingAudioComponent->SetIntParameter(FName("sfx_LandingMaterial"), 0);
			}
			OnWalkingBpEvent();
			LandingAudioComponent->Play();
		}

		else
		{
			LandingAudioComponent->Stop();
		}
	}
	//this->WallrunEnd();

}

void AMyProjectCharacter::VaultSound()
{
	VaultAudioComponent->Play();
}

void AMyProjectCharacter::ClimbSound()
{
	ClimbAudioComponent->Play();
}

					// Keyboard left Shift //



				//////////////////////////////////////
				//////////	  Functions     //////////
				//////////////////////////////////////

void AMyProjectCharacter::Damage(int damage, FVector damageCauser)
{
	if (godMode == false || !this->blockInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ressources at : %f %"), this->Health);
		Health = Health - damage;
		this->OnDamageBPEvent();
		isHit = true;
		world->GetTimerManager().SetTimer(delay, this, &AMyProjectCharacter::GotHit, this->lastTimeHitDelay, false);
		playerpos = this->GetActorForwardVector();
		playerpos.Normalize();
		damageCauser = damageCauser - this->GetActorForwardVector();
		damageCauser.Normalize();
		playerpos = playerpos * FVector(1, 1, 0);
		damageCauser = damageCauser * FVector(1, 1, 0);
		hitAngle = FMath::Acos(FVector::DotProduct(damageCauser, playerpos));
		hitAngle = FMath::RadiansToDegrees(hitAngle);
		UE_LOG(LogTemp, Warning, TEXT("Hit Angle: %f %"), this->hitAngle);
		NormalHitAudioComponent->Play();
		if (Health <= 30.0f)
		{
			if (!gothitlessthan30)
			{
				gothitlessthan30 = true;
				PlayerHitAudioComponent->Play();
			}
		}

		if (Health <= 0.0f)
		{
			if (dead == false)
			{
				DeathAudioComponent->Play();
				OnIsDeadBpEvent();
				LMBReleased();
				dead = true;
				//this->playerController->UnPossess();
				this->AActor::DisableInput(Cast<APlayerController>(this));
				this->playerController->SetIgnoreLookInput(true);
				this->playerController->SetIgnoreMoveInput(true);
				this->PlayDeathAnim();
				world->GetTimerManager().SetTimer(respawn, this, &AMyProjectCharacter::Respawn, 1.0f, false);
			}
		}
	}
}

void AMyProjectCharacter::GotHit()
{
	isHit = false;
}

void AMyProjectCharacter::NotMoving()
{
	//ismovingTimer = false;
	islosingHealth = true;
	UE_LOG(LogTemp, Warning, TEXT("1"));
	Health = Health - crankDamage;
	this->OnCrankDamageBpEvent();
}
void AMyProjectCharacter::LosingHealth()
{
	if (!this->blockInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("losingHealth"));
		Health = Health - crankDamage;
		this->OnCrankDamageBpEvent();
		if (Health <= crankHealthThreshhold /*&& isHit == false*/)
		{
			Health = crankHealthThreshhold;
			//if (dead == false)
			//{
			//	OnCrankDamageBpEvent();
			//	LMBReleased();
			//	dead = true;
			//	//this->playerController->UnPossess();
			//	this->AActor::DisableInput(Cast<APlayerController>(this));
			//	this->playerController->SetIgnoreLookInput(true);
			//	this->playerController->SetIgnoreMoveInput(true);
			//	this->PlayDeathAnim();
			//	world->GetTimerManager().SetTimer(respawn, this, &AMyProjectCharacter::Respawn, 1.0f, false);
			//}
		}
		if (Health <= 30.0f)
		{
			if (!gothitlessthan30)
			{
				gothitlessthan30 = true;
				PlayerHitAudioComponent->Play();
			}
		}
	}
}

void AMyProjectCharacter::SetRespawn(FVector spawnVector, FRotator spawnRotator)
{
	spawnRotation = spawnRotator;
	spawnPoint = spawnVector;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *this->spawnRotation.ToString());
}

void AMyProjectCharacter::Respawn()
{
	Health = MaxHealth;
	Reload();
	this->OnDamageBPEvent();
	this->OnHealthRechargeBPEvent();
	TeleportTo(spawnPoint, spawnRotation, false, true);
	dead = false;
	//this->playerController->Possess(this);
	this->AActor::EnableInput(Cast<APlayerController>(this));
	this->playerController->SetIgnoreLookInput(false);
	this->playerController->SetIgnoreMoveInput(false);
	OnRespawnBpEvent();

	for (TActorIterator<ATP_ThirdPersonCharacter> it(GetWorld()); it; ++it)
	{
		it->EnemyRespawn();
	}
}

void AMyProjectCharacter::RespawnSound()
{
	DeathAudioComponent->Play();
}

void AMyProjectCharacter::Healthrecharge()
{
	if (this->Health < this->MaxHealth)
	{

		Health += healthPerDelay;
		OnHealthRechargeBPEvent();

		if (this->Health >= this->MaxHealth)
		{
			this->OnHealthMaxed();
		}
	}
}

void AMyProjectCharacter::RessoourceRefill(float amount)
{
	ressource = ressource + amount;
	OnResourceChange();
}

void AMyProjectCharacter::Slide()
{
	acceleration = this->movementComponent->GetCurrentAcceleration();
	//isFalling = this->movementComponent->IsFalling();
	ishiftButtonPressed = true;
	if (acceleration != FVector(0,0,0))
	{
		OnSlideBpEvent();
		sliding = true;
		this->movementComponent->GroundFriction = 0.0f;
		this->movementComponent->BrakingDecelerationWalking = 0.0f;
		this->movementComponent->BrakingFrictionFactor = 0.0f;

		FVector launchCharacterVector = this->FirstPersonCameraComponent->GetForwardVector();
		launchCharacterVector = launchCharacterVector * 1000;

		this->LaunchCharacter(FVector(
			launchCharacterVector.X,
			launchCharacterVector.Y,
			0.0f),
			true,
			false
		);

		RevertedSlideCam();
		SlideAudioComponent->Play();
	}
	else if (acceleration == FVector(0, 0, 0))
	{
		this->movementComponent->MaxAcceleration = 1500;
		this->movementComponent->MaxWalkSpeed = 400;
		RevertedSlideCam();
	}
}

void AMyProjectCharacter::EndSlide()
{
	OnSlideEndBpEvent();
	sliding = false;
	ishiftButtonPressed = false;
	this->movementComponent->MaxAcceleration = 3000;
	this->movementComponent->MaxWalkSpeed = walkSpeed;
	this->movementComponent->GroundFriction = 8;
	this->movementComponent->BrakingDecelerationWalking = 2048;
	this->movementComponent->BrakingFrictionFactor = 2;
	SlideCam();
	SlideAudioComponent->Stop();
}

void AMyProjectCharacter::SlideCam()
{
	this->movementComponent->GravityScale = gravitation;
	this->slideradiusTimeline->Play();
	this->slideheightTimeline->Play();

}

void AMyProjectCharacter::RevertedSlideCam()
{
	//this->movementComponent->GravityScale = gravitation;
	this->slideradiusTimeline->Reverse();
	this->slideheightTimeline->Reverse();
}

void AMyProjectCharacter::Sprint()
{
	this->movementComponent->MaxWalkSpeed = sprintSpeed;
	canSprint = true;
}

void AMyProjectCharacter::EndSprint()
{
	this->movementComponent->MaxWalkSpeed = walkSpeed;
	canSprint = false;
}

void AMyProjectCharacter::BulletCooldown()
{
	this->isBulletFired = false;
}

void AMyProjectCharacter::SpawnBullet()
{
	if (this->isVaulting == true)
	{
		shootingState = ShootState::RightShooting;
	}

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
		if (shootingState == ShootState::RightShooting)
		{
			UE_LOG(LogTemp, Warning, TEXT("Right"));
			shootingState = ShootState::LeftShooting;
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(												// Set a Rotater from MuzzelLocation to Ray Imapact Point
				FP_MuzzleLocationRight->GetComponentTransform().GetLocation(),												// Get Muzzel Location in World
				hitResult.ImpactPoint);																					// Get Impact Point in World

			FActorSpawnParameters spawnInfo;																			// Spawn Info
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;					// Spawn Actor Always
			spawnInfo.Owner = this;																						// Owner of Spawned Actor is this Class
			spawnInfo.Instigator = this;																				// if you spawn a projectile and pass over instigator as say, your pawn. then when the projectile wants to do damage, you can pass it over to the damage function. So you know who fire the projectile, you can then decide if you want friendly fire, and all sorts of things can be done after you know this bit of info.

			AMyProjectProjectile* projectile = world->SpawnActor<AMyProjectProjectile>(		// Spawn the Bullet
				playerProjectile,															// SubClass
				FP_MuzzleLocationRight->GetComponentTransform().GetLocation(),				// SpawnLocation
				newRotation,																// SpawnRotation
				spawnInfo);																	// Set Spawn Info

			projectile->Initialize(this);
			OnBulletSpawnBpEvent();
			// try and play a firing animation if specified
			if (fireAnimationRight != NULL)
			{
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(fireAnimationRight, 1.f);
				}
			}
		}
		else if (shootingState == ShootState::LeftShooting)
		{
			UE_LOG(LogTemp, Warning, TEXT("Left"));
			shootingState = ShootState::RightShooting;
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(												// Set a Rotater from MuzzelLocation to Ray Imapact Point
				FP_MuzzleLocationLeft->GetComponentTransform().GetLocation(),												// Get Muzzel Location in World
				hitResult.ImpactPoint);																					// Get Impact Point in World

			FActorSpawnParameters spawnInfo;																			// Spawn Info
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;					// Spawn Actor Always
			spawnInfo.Owner = this;																						// Owner of Spawned Actor is this Class
			spawnInfo.Instigator = this;																				// if you spawn a projectile and pass over instigator as say, your pawn. then when the projectile wants to do damage, you can pass it over to the damage function. So you know who fire the projectile, you can then decide if you want friendly fire, and all sorts of things can be done after you know this bit of info.

			AMyProjectProjectile* projectile = world->SpawnActor<AMyProjectProjectile>(		// Spawn the Bullet
				playerProjectile,															// SubClass
				FP_MuzzleLocationLeft->GetComponentTransform().GetLocation(),				// SpawnLocation
				newRotation,																// SpawnRotation
				spawnInfo);
			projectile->Initialize(this);
			OnBulletSpawnBpEvent();

			if (fireAnimationLeft != NULL)
			{
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(fireAnimationLeft, 1.f);
				}
			}
		}
		ShootAudioComponent->Play();
	}
	else																				// Ray does not hit anything
	{

		if (shootingState == ShootState::RightShooting)
		{
			UE_LOG(LogTemp, Warning, TEXT("Right"));
			shootingState = ShootState::LeftShooting;
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(												// Set a Rotater from MuzzelLocation to Ray Imapact Point
				FP_MuzzleLocationRight->GetComponentTransform().GetLocation(),												// Get Muzzel Location in World
				hitResult.TraceEnd);																					// Get Trace end in World

			FActorSpawnParameters spawnInfo;																			// Spawn Info
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;					// Spawn Actor Always
			spawnInfo.Owner = this;																						// Owner of Spawned Actor is this Class
			spawnInfo.Instigator = this;																				// if you spawn a projectile and pass over instigator as say, your pawn. then when the projectile wants to do damage, you can pass it over to the damage function. So you know who fire the projectile, you can then decide if you want friendly fire, and all sorts of things can be done after you know this bit of info.

			AMyProjectProjectile* projectile = world->SpawnActor<AMyProjectProjectile>(		// Spawn the Bullet
				playerProjectile,															// SubClass
				FP_MuzzleLocationRight->GetComponentTransform().GetLocation(),				// SpawnLocation
				newRotation,																// SpawnRotation
				spawnInfo);
			projectile->Initialize(this);// Set Spawn Info
			OnBulletSpawnBpEvent();

			// try and play a firing animation if specified
			if (fireAnimationRight != NULL)
			{
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(fireAnimationRight, 1.f);
				}
			}
		}
		else if (shootingState == ShootState::LeftShooting)
		{
			UE_LOG(LogTemp, Warning, TEXT("Left"));
			shootingState = ShootState::RightShooting;
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(												// Set a Rotater from MuzzelLocation to Ray Imapact Point
				FP_MuzzleLocationLeft->GetComponentTransform().GetLocation(),												// Get Muzzel Location in World
				hitResult.TraceEnd);																					// Get Trace end in World

			FActorSpawnParameters spawnInfo;																			// Spawn Info
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;					// Spawn Actor Always
			spawnInfo.Owner = this;																						// Owner of Spawned Actor is this Class
			spawnInfo.Instigator = this;																				// if you spawn a projectile and pass over instigator as say, your pawn. then when the projectile wants to do damage, you can pass it over to the damage function. So you know who fire the projectile, you can then decide if you want friendly fire, and all sorts of things can be done after you know this bit of info.

			AMyProjectProjectile* projectile = world->SpawnActor<AMyProjectProjectile>(		// Spawn the Bullet
				playerProjectile,															// SubClass
				FP_MuzzleLocationLeft->GetComponentTransform().GetLocation(),				// SpawnLocation
				newRotation,																// SpawnRotation
				spawnInfo);
			projectile->Initialize(this);// Set Spawn Info
			OnBulletSpawnBpEvent();

			if (fireAnimationLeft != NULL)
			{
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(fireAnimationLeft, 1.f);
				}
			}
		}
		ShootAudioComponent->Play();
	}

	////////////////////////////////////////////////
	////////////	 Play Sound and Animation
	////////////////////////////////////////////////

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

// Wallrun (MovePlayer)
void AMyProjectCharacter::WallrunLaunch()
{
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
		OnWallRunBpEvent();
		if (hitResult.IsValidBlockingHit() == true)
		{
			if (hitResult.GetActor()->ActorHasTag("RunWall"))	// Wallrun if you look at wall && the wall is on your Right side
			{
				FVector playerPosition = this->GetTransform().GetLocation();
				this->playerDirection = this->playerDirection * 1000;

				FVector launchCharacterVector = this->wallRunDirection - playerPosition;
				launchCharacterVector = launchCharacterVector + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);			// Launches Character in the Desiert Direction

				GravitationOff();
			}
			else if (hitResult.GetActor()->ActorHasTag("RunWallUp"))
			{
				WallrunUp = true;
				FVector playerPosition = this->GetTransform().GetLocation();
				this->playerDirection = this->playerDirection * 1000;

				FVector launchCharacterVector = this->wallRunDirection - playerPosition;
				launchCharacterVector = launchCharacterVector + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);			// Launches Character in the Desiert Direction

				GravitationOff();
			}
			else												// wallrun if u dont look at wall && the wall is on your Right side
			{
				this->playerDirection = this->playerDirection * 1000;
				FVector launchCharacterVector = this->playerRightVector * 1000 + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);			// Launches Character in the Desiert Direction

				GravitationOff();
			}
		}
		else
		{
			this->playerDirection = this->playerDirection * 1000;
			FVector launchCharacterVector = this->playerRightVector * 1000 + this->playerDirection;

			this->LaunchCharacter(launchCharacterVector, true, true);				// Launches Character in the Desiert Direction

			GravitationOff();
		}
	}
	else
	{
		OnWallRunBpEvent();
		if (hitResult.IsValidBlockingHit() == true)
		{
			if (hitResult.GetActor()->ActorHasTag("RunWall"))	// Wallrun if you look at wall && the wall is on your Left side
			{
				FVector playerPosition = this->GetTransform().GetLocation();
				this->playerDirection = this->playerDirection * 1000;

				FVector launchCharacterVector = this->wallRunDirection - playerPosition;
				launchCharacterVector = launchCharacterVector + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);			// Launches Character in the Desiert Direction

				GravitationOff();
			}
			else if (hitResult.GetActor()->ActorHasTag("RunWallUp"))												 
			{
				WallrunUp = true;
				FVector playerPosition = this->GetTransform().GetLocation();
				this->playerDirection = this->playerDirection * 1000;

				FVector launchCharacterVector = this->wallRunDirection - playerPosition;
				launchCharacterVector = launchCharacterVector + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);			// Launches Character in the Desiert Direction

				GravitationOff();
			}
			else
			{
				this->playerDirection = this->playerDirection * 1000;
				FVector launchCharacterVector = this->playerRightVector * -1000 + this->playerDirection;

				this->LaunchCharacter(launchCharacterVector, true, true);			// Launches Character in the Desiert Direction

				GravitationOff();
			}
		}
		else
		{
			this->playerDirection = this->playerDirection * 1000;
			FVector launchCharacterVector = this->playerRightVector * -1000 + this->playerDirection;

			this->LaunchCharacter(launchCharacterVector, true, true);				// Launches Character in the Desiert Direction

			GravitationOff();
		}
	}
}

void AMyProjectCharacter::GravitationOn()
{
	this->movementComponent->GravityScale = this->gravitation;
	this->movementComponent->AirControl = this->airControll;
	this->movementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 0.0f));
	this->isOnWall = false;
	this->isWallRight = false;
	this->isWallLeft = false;
	this->wallrunDoOnce = true;
}

//// Called after LaunchCharacter
void AMyProjectCharacter::GravitationOff()
{
	this->movementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));			// Locks the Axis
	this->movementComponent->GravityScale = 0.2f;											// can be 0.0f aswell 
	this->movementComponent->AirControl = 0.0;												// Player Cant control in the Air

	this->wallrunTimeline->Stop();

	world->GetTimerManager().SetTimer(wallrunHandle, this, &AMyProjectCharacter::WallrunRetriggerableDelay, this->wallrunDuration, false);

}

// The Delay ends the Wallrun, can be retrigged by switching the wall
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

			this->camTiltRightTimeline->Reverse();
			this->camTiltLeftTimeline->Reverse();
		}
	}
}


// Resets Values to Normal
void AMyProjectCharacter::WallrunEnd()
{
	OnWallRunEndBpEvent();

	if (this->isWallRight == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		FVector forwardVector = this->GetActorForwardVector() * this->wallJumpForceForward;
		FVector sideVector = this->GetActorRightVector() * this->helperWallJumpNegativeFloat;
		FVector launchVector = sideVector + forwardVector;

		this->LaunchCharacter(FVector(							// Jump when wall is on you right side
			launchVector.X,
			launchVector.Y,
			this->jumpHeightOnWall),
			true,
			true
		);

		JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 2);
		JumpAudioComponent->Play();
	}
	else if (this->isWallLeft == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		FVector forwardVector = this->GetActorForwardVector() * this->wallJumpForceForward;
		FVector sideVector = this->GetActorRightVector() * this->wallJumpForce;
		FVector launchVector = sideVector + forwardVector;

		this->LaunchCharacter(FVector(						// Jump when wall is on your Left side
			launchVector.X,
			launchVector.Y,
			this->jumpHeightOnWall),
			true,
			true
		);

		JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 2);
		JumpAudioComponent->Play();
	}
	this->movementComponent->GravityScale = this->gravitation;
	this->movementComponent->AirControl = this->airControll;
	this->movementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 0.0f));
	this->isOnWall = false;
	this->isWallRight = false;
	this->isWallLeft = false;
	this->wallrunDoOnce = true;

	WallrunUp = false;
}
void AMyProjectCharacter::WallrunEndUp()
{
	OnWallRunEndBpEvent();

	if (this->isWallRight == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		FVector forwardVector = this->GetActorForwardVector() * this->wallJumpForceForward;
		FVector sideVector = this->GetActorRightVector() * this->helperWallJumpNegativeFloat;
		FVector launchVector = sideVector + forwardVector;

		this->LaunchCharacter(FVector(							// Jump when wall is on you right side
			launchVector.X,
			launchVector.Y,
			this->jumpHeightOnWallUp),
			true,
			true
		);

		JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 2);
		JumpAudioComponent->Play();
	}
	else if (this->isWallLeft == true)
	{
		this->movementComponent->GravityScale = gravitation;	// Set Gravity

		FVector forwardVector = this->GetActorForwardVector() * this->wallJumpForceForward;
		FVector sideVector = this->GetActorRightVector() * this->wallJumpForce;
		FVector launchVector = sideVector + forwardVector;

		this->LaunchCharacter(FVector(						// Jump when wall is on your Left side
			launchVector.X,
			launchVector.Y,
			this->jumpHeightOnWallUp),
			true,
			true
		);

		JumpAudioComponent->SetIntParameter(FName("sfx_JumpMaterial"), 2);
		JumpAudioComponent->Play();
	}
	this->movementComponent->GravityScale = this->gravitation;
	this->movementComponent->AirControl = this->airControll;
	this->movementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 0.0f));
	this->isOnWall = false;
	this->isWallRight = false;
	this->isWallLeft = false;
	this->wallrunDoOnce = true;

	WallrunUp = false;
}

						//////////////////////////////////////
						//////////	  Collision     //////////
						//////////////////////////////////////

/// Main Start Walldetection
void AMyProjectCharacter::OnWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResul)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() && this->isOnLadder == false || otherActor->ActorHasTag("RunWallUp") && this->movementComponent->IsFalling() && this->isOnLadder == false)		// Check if the Wall has the right TAG and if the player is in the air
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
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->wallrunTimeline->Stop();																			// Stops Timeline

		if (this->wallCollisionCounter >= 1)
		{
			this->wallCollisionCounter--;																		// Decrements Counter when its higher than 0

			if (this->wallCollisionCounter == 0)
			{
				this->GravitationOn();
				OnWallRunEndBpEvent();
				//this->WallrunEnd();																				// Call End Wallrun
			}
		}
	}
	else if (otherActor->ActorHasTag("RunWallUp") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->wallrunTimeline->Stop();																			// Stops Timeline

		if (this->wallCollisionCounter >= 1)
		{
			this->wallCollisionCounter--;																		// Decrements Counter when its higher than 0

			if (this->wallCollisionCounter == 0)
			{
				this->GravitationOn();
				OnWallRunEndBpEvent();
				//this->WallrunEndUp();																				// Call End Wallrun
			}
		}
	}
}

/// Right Walldetector Begin
void AMyProjectCharacter::OnRightWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResul)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->isWallLeft = false;					// set the bool false so it only can on of them be true (safty first)
		this->isWallRight = true;	

		this->currentCamRotation = this->FirstPersonCameraComponent->GetComponentRotation();
		this->tiltedCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, -20.0f);
		this->normalCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 0.0f);

		this->camTiltRightTimeline->Play();
	}
	else if (otherActor->ActorHasTag("RunWallUp") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->isWallLeft = false;					// set the bool false so it only can on of them be true (safty first)
		this->isWallRight = true;

		this->WallrunUp = true;

		this->currentCamRotation = this->FirstPersonCameraComponent->GetComponentRotation();
		this->tiltedCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, -20.0f);
		this->normalCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 0.0f);

		this->camTiltRightTimeline->Play();
	}
}
/// Right Walldetector End
void AMyProjectCharacter::EndRightWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() && this->isOnLadder == false || otherActor->ActorHasTag("RunWallUp") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->isWallLeft = false;					
		this->isWallRight = false;

		this->WallrunUp = false;

		this->currentCamRotation = this->FirstPersonCameraComponent->GetComponentRotation();
		this->tiltedCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, -20.0f);
		this->normalCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 0.0f);

		this->camTiltRightTimeline->Reverse();
	}
}

/// Left Walldetector Begin
void AMyProjectCharacter::OnLeftWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResul)
{

	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->isWallRight = false;						// set the bool false so it only can on of them be true (safty first)
		this->isWallLeft = true;		

		this->currentCamRotation = this->FirstPersonCameraComponent->GetComponentRotation();
		this->tiltedCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 20.0f);
		this->normalCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 0.0f);

		this->camTiltLeftTimeline->Play();
	}
	else if (otherActor->ActorHasTag("RunWallUp") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->isWallRight = false;						// set the bool false so it only can on of them be true (safty first)
		this->isWallLeft = true;

		this->WallrunUp = true;

		this->currentCamRotation = this->FirstPersonCameraComponent->GetComponentRotation();
		this->tiltedCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 20.0f);
		this->normalCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 0.0f);

		this->camTiltLeftTimeline->Play();
	}
}
/// Left Walldetector End
void AMyProjectCharacter::EndLeftWallDetected(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->ActorHasTag("RunWall") && this->movementComponent->IsFalling() && this->isOnLadder == false || otherActor->ActorHasTag("RunWallUp") && this->movementComponent->IsFalling() && this->isOnLadder == false)
	{
		this->isWallLeft = false;
		this->isWallRight = false;
		this->WallrunUp = false;

		this->currentCamRotation = this->FirstPersonCameraComponent->GetComponentRotation();
		this->tiltedCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 20.0f);
		this->normalCamRotation = FRotator(currentCamRotation.Pitch, currentCamRotation.Yaw, 0.0f);

		this->camTiltLeftTimeline->Reverse();
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
				if (hitResult.GetActor()->ActorHasTag("RunWall") || hitResult.GetActor()->ActorHasTag("RunWallUp"))
				{
					FVector rayHitLocation = hitResult.Location;
					FVector wallForwardVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * 10;
					FVector wallBackwardsVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * -10;
					FVector cameraRotation = this->FirstPersonCameraComponent->GetComponentTransform().GetRotation().GetForwardVector() * 1000 + this->FirstPersonCameraComponent->GetComponentTransform().GetLocation();
					wallForwardVector = wallForwardVector + rayHitLocation;
					wallBackwardsVector = wallBackwardsVector + rayHitLocation;

					FVector forwardVector = cameraRotation - wallForwardVector;
					FVector backwardsVector = cameraRotation - wallBackwardsVector;

					if (forwardVector.Size() < backwardsVector.Size())	// Check in witch direction the player is watching (Set the Vector that is Closer) 
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
				if (hitResult.GetActor()->ActorHasTag("RunWall") || hitResult.GetActor()->ActorHasTag("RunWallUp"))
				{
					FVector rayHitLocation = hitResult.Location;
					FVector wallForwardVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * 10;
					FVector wallBackwardsVector = hitResult.GetActor()->GetTransform().GetRotation().GetForwardVector() * -10;
					FVector cameraRotation = this->FirstPersonCameraComponent->GetComponentTransform().GetRotation().GetForwardVector() * 1000 + this->FirstPersonCameraComponent->GetComponentTransform().GetLocation();
					wallForwardVector = wallForwardVector + rayHitLocation;
					wallBackwardsVector = wallBackwardsVector + rayHitLocation;

					FVector forwardVector = cameraRotation - wallForwardVector;
					FVector backwardsVector = cameraRotation - wallBackwardsVector;

					if (forwardVector.Size() < backwardsVector.Size())	// Check in witch direction the player is watching (Set the Vector that is Closer) 
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

void AMyProjectCharacter::TiltCamRightFloatReturn(float value)
{
	if (this->isWallRight == true)
	{
		FRotator lerpCamTilt = FMath::Lerp(
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->currentCamRotation.Roll),
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->tiltedCamRotation.Roll),
			value
		);

		this->playerController->SetControlRotation(lerpCamTilt);
	}
	else
	{
		FRotator lerpCamTilt = FMath::Lerp(
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->normalCamRotation.Roll),
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->currentCamRotation.Roll),
			value
		);

		this->playerController->SetControlRotation(lerpCamTilt);
	}
}

void AMyProjectCharacter::TiltCamLeftFloatReturn(float value)
{
	if (this->isWallLeft == true)
	{
		FRotator lerpCamTilt = FMath::Lerp(
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->currentCamRotation.Roll),
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->tiltedCamRotation.Roll),
			value
		);
		this->playerController->SetControlRotation(lerpCamTilt);
	}
	else
	{
		FRotator lerpCamTilt = FMath::Lerp(
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->normalCamRotation.Roll),
			FRotator(this->FirstPersonCameraComponent->GetComponentRotation().Pitch, this->FirstPersonCameraComponent->GetComponentRotation().Yaw, this->currentCamRotation.Roll),
			value
		);
		this->playerController->SetControlRotation(lerpCamTilt);
	}
}

void AMyProjectCharacter::SlideHeightFloatReturn(float height)
{
	if (sliding)
	{
		this->capsuleComponent->SetCapsuleHalfHeight(height, true);
	}
	else if(!sliding)
	{
		this->capsuleComponent->SetCapsuleHalfHeight(height, true);
	}
}

void AMyProjectCharacter::SlideRadiusFloatReturn(float radius)
{
	if (sliding)
	{
		this->capsuleComponent->SetCapsuleRadius(radius, true);
	}
	else if (!sliding)
	{
		this->capsuleComponent->SetCapsuleRadius(radius, true);
	}
}

void AMyProjectCharacter::RunningAudioBegin()
{
	if (RunningAudioComponent->IsPlaying() == false)
	{
		RunningAudioComponent->FadeIn(FMath::RandRange(3.0f, 5.0f), 1.0, 0.0);
	}
}

void AMyProjectCharacter::RunningAudioEnd()
{
	RunningAudioComponent->FadeOut(FMath::RandRange(3.0f, 5.0f), 0.0);
}

// ALI

void AMyProjectCharacter::SetShootingLeft()
{
	shootingState = ShootState::LeftShooting;
}

void AMyProjectCharacter::SetShootingRight()
{
	shootingState = ShootState::RightShooting;
}

void AMyProjectCharacter::OnIntroBPEvent()
{
	Dialogue_IntroAudioComponent->Play();
}

void AMyProjectCharacter::OnHalfwayBPEvent()
{
	Dialogue_HalfWayAudioComponent->Play();
}

void AMyProjectCharacter::OnAlmostBPEvent()
{
	Dialogue_AlmostAudioComponent->Play();
}

void AMyProjectCharacter::OnOutroBPEvent()
{
	Dialogue_OutroAudioComponent->Play();
}
