// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHitMetal.h"


// Sets default values
AMyHitMetal::AMyHitMetal()
{
	world = GetWorld();

	//HitMetal
	static ConstructorHelpers::FObjectFinder<USoundCue> HitMetalCue(TEXT("'/Game/Sound/SFX/HIts/sfx_HitMetal'"));
	HitMetalAudioCue = HitMetalCue.Object;
	HitMetalAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitMetalAudioComp"));
	HitMetalAudioComponent->bAutoActivate = false;
	HitMetalAudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyHitMetal::BeginPlay()
{
	Super::BeginPlay();
	
	HitMetalAudioComponent->Play();

	world->GetTimerManager().SetTimer(cleanup, this, &AMyHitMetal::Cleanup, cleanupDelay, false);
	UE_LOG(LogTemp, Warning, TEXT("HitHead"));
}

void AMyHitMetal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HitMetalAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		HitMetalAudioComponent->SetSound(HitMetalAudioCue);
	}
}

// Called every frame
void AMyHitMetal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	///Sounds Slowmo
	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	HitMetalAudioComponent->SetFloatParameter(FName("sfx_HitMetalSlowmo"), soundTimeDilation);
}

void AMyHitMetal::Cleanup()
{
	Destroy();
}

