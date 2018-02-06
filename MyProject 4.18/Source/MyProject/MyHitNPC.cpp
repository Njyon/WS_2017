// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHitNPC.h"


// Sets default values
AMyHitNPC::AMyHitNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	world = GetWorld();

	//HitNPC
	static ConstructorHelpers::FObjectFinder<USoundCue> HitNPCCue(TEXT("'/Game/Sound/SFX/HIts/sfx_EnemyHit'"));
	HitNPCAudioCue = HitNPCCue.Object;
	HitNPCAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitNPCAudioComp"));
	HitNPCAudioComponent->bAutoActivate = false;
	HitNPCAudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyHitNPC::BeginPlay()
{
	Super::BeginPlay();

	HitNPCAudioComponent->Play();

	world->GetTimerManager().SetTimer(cleanup, this, &AMyHitNPC::Cleanup, cleanupDelay, false);
	UE_LOG(LogTemp, Warning, TEXT("HitNPC"));
}

void AMyHitNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HitNPCAudioCue->IsValidLowLevelFast())					//ShootSound
	{
		HitNPCAudioComponent->SetSound(HitNPCAudioCue);
	}
}

// Called every frame
void AMyHitNPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	///Sounds Slowmo
	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	HitNPCAudioComponent->SetFloatParameter(FName("sfx_HitNPCSlowmo"), soundTimeDilation);
}

void AMyHitNPC::Cleanup()
{
	Destroy();
}

