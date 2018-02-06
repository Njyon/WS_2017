// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHitHead.h"


// Sets default values
AMyHitHead::AMyHitHead()
{
 //	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	world = GetWorld();

	//HitHead
	static ConstructorHelpers::FObjectFinder<USoundCue> HitHeadCue(TEXT("'/Game/Sound/SFX/HIts/sfx_HitHead'"));
	HitHeadAudioCue = HitHeadCue.Object;
	HitHeadAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitHeadAudioComp"));
	HitHeadAudioComponent->bAutoActivate = false;
	HitHeadAudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyHitHead::BeginPlay()
{
	Super::BeginPlay();

	HitHeadAudioComponent->Play();
	
	world->GetTimerManager().SetTimer(cleanup, this, &AMyHitHead::Cleanup, cleanupDelay, false);
	UE_LOG(LogTemp, Warning, TEXT("HitHead"));
}

void AMyHitHead::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HitHeadAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		HitHeadAudioComponent->SetSound(HitHeadAudioCue);
	}
}

// Called every frame
void AMyHitHead::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	///Sounds Slowmo
	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	HitHeadAudioComponent->SetFloatParameter(FName("sfx_HitHeadSlowmo"), soundTimeDilation);
}

void AMyHitHead::Cleanup()
{
	Destroy();
}

