// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHitWall.h"


// Sets default values
AMyHitWall::AMyHitWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	world = GetWorld();

	//HitWall
	static ConstructorHelpers::FObjectFinder<USoundCue> HitWallCue(TEXT("'/Game/Sound/SFX/HIts/sfx_HitWall'"));
	HitWallAudioCue = HitWallCue.Object;
	HitWallAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitWallAudioComp"));
	HitWallAudioComponent->bAutoActivate = false;
	HitWallAudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyHitWall::BeginPlay()
{
	Super::BeginPlay();

	HitWallAudioComponent->Play();

	world->GetTimerManager().SetTimer(cleanup, this, &AMyHitWall::Cleanup, cleanupDelay, false);
	
}

void AMyHitWall::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HitWallAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		HitWallAudioComponent->SetSound(HitWallAudioCue);
	}
}

void AMyHitWall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	///Sounds Slowmo
	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	HitWallAudioComponent->SetFloatParameter(FName("sfx_HitWallSlowmo"), soundTimeDilation);
}

void AMyHitWall::Cleanup()
{
	Destroy();
}

