// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectCheckpoint.h"


// Sets default values
AMyProjectCheckpoint::AMyProjectCheckpoint()
{

	world = GetWorld();

	static ConstructorHelpers::FClassFinder<AMyEnemySpawnSound> SpawnSoundSound(TEXT("'/Game/Blueprints/Player/Behaviour/EnemySpawnSound'"));
	if (SpawnSoundSound.Class != NULL)
	{
		EnemySpawnSound = SpawnSoundSound.Class;
	}

	//CheckpointSound
	static ConstructorHelpers::FObjectFinder<USoundCue> CheckpointSoundCue(TEXT("'/Game/Sound/SFX/other/sfx_Checkpoint'"));
	CheckpointSoundAudioCue = CheckpointSoundCue.Object;
	CheckpointSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CheckpointSoundAudioComp"));
	CheckpointSoundAudioComponent->bAutoActivate = false;
	CheckpointSoundAudioComponent->SetupAttachment(RootComponent);

	//Spawn
	static ConstructorHelpers::FObjectFinder<USoundCue> SpawnCue(TEXT("'/Game/Sound/SFX/other/sfx_EnemySpawn'"));
	SpawnAudioCue = SpawnCue.Object;
	SpawnAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SpawnCueAudioComp"));
	SpawnAudioComponent->bAutoActivate = false;
	SpawnAudioComponent->SetupAttachment(RootComponent);

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCheckpoint::OnOverlap);


	SoundSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SoundSpawn"));
	SoundSpawnLocation->SetupAttachment(RootComponent);
	SoundSpawnLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Set as root component
	RootComponent = CollisionComp;

}


void AMyProjectCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	vector = GetActorLocation();
	rotator = GetActorRotation();

	/*vector = spawn->GetComponentLocation();
	rotator = spawn->GetComponentRotation();

	UE_LOG(LogTemp, Warning, TEXT("Spawn Location is %s"), *spawn->GetComponentLocation().ToString());*/

	this->lenght = spawns.Num();
}

void AMyProjectCheckpoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CheckpointSoundAudioCue->IsValidLowLevelFast())					//WallrunSound
	{
		CheckpointSoundAudioComponent->SetSound(CheckpointSoundAudioCue);
	}
	if (SpawnAudioCue->IsValidLowLevelFast())					//WallrunSound
	{
		SpawnAudioComponent->SetSound(SpawnAudioCue);
	}
}

void AMyProjectCheckpoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	soundTimeDilation = FMath::Clamp(UGameplayStatics::GetGlobalTimeDilation(world), 0.0f, 1.0f);
	SpawnAudioComponent->SetFloatParameter(FName("sfx_EnemySpawnSlowmo"), soundTimeDilation);

}

void AMyProjectCheckpoint::NextCheckpoint()
{
	if (nextCP != NULL)
	{
		nextCP->ActivateNextCP();
	}
}

  void AMyProjectCheckpoint::OnOverlap(class UPrimitiveComponent* hitComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResult)
{
	AMyProjectCharacter* hittedplayer = Cast<AMyProjectCharacter>(otherActor);

	if ((otherActor != NULL) && (otherActor != this) && hittedplayer != NULL)
	{
		if (!hasUsed)
		{
			vector = hittedplayer->GetActorLocation();

			this->CheckpointSoundAudioComponent->Play();
			this->SpawnAudioComponent->Play();
			UE_LOG(LogTemp, Warning, TEXT("success"));
			this->hasUsed = true;
			hittedplayer->SetRespawn(vector, rotator);
			OnCheckpoint();
			NextCheckpoint();

			/*if (!firstCP)
			{*/
				/*FActorSpawnParameters spawnInfo;
				spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AMyEnemySpawnSound* soundspawn = world->SpawnActor<AMyEnemySpawnSound>(
					EnemySpawnSound,
					SoundSpawnLocation->GetComponentTransform().GetLocation(),
					FRotator(0, 0, 0),
					spawnInfo);*/
			//}

			for (int i = 0; i <= this->lenght + 1; i++)
			{
				amountOfEnemys--;

				if (i == this->lenght)
				{
					i = 0;
				}
				if (amountOfEnemys >= 0)
				{
					spawns[i]->spawnQue += 1;
					spawns[i]->Spawnenemy();
				}
				else if (amountOfEnemys <= 0)
				{
					return;
				}
			}

		}
	}
}
