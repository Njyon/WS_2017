// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHitSand.h"


// Sets default values
AMyHitSand::AMyHitSand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	world = GetWorld();

	//HitWall
	static ConstructorHelpers::FObjectFinder<USoundCue> HitSandCue(TEXT("'/Game/Sound/SFX/HIts/sfx_HitSand'"));
	HitSandAudioCue = HitSandCue.Object;
	HitSandAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSandAudioComp"));
	HitSandAudioComponent->bAutoActivate = false;
	HitSandAudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyHitSand::BeginPlay()
{
	Super::BeginPlay();

	HitSandAudioComponent->Play();

	world->GetTimerManager().SetTimer(cleanup, this, &AMyHitSand::Cleanup, cleanupDelay, false);
	
}

void AMyHitSand::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HitSandAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		HitSandAudioComponent->SetSound(HitSandAudioCue);
	}
}

// Called every frame
void AMyHitSand::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	///Sounds Slowmo
	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	HitSandAudioComponent->SetFloatParameter(FName("sfx_HitSandSlowmo"), soundTimeDilation);

}

void AMyHitSand::Cleanup()
{
	Destroy();
}

