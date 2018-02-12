// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectCheckpoint.h"


// Sets default values
AMyProjectCheckpoint::AMyProjectCheckpoint()
{
	world = GetWorld();

	//CheckpointSound
	static ConstructorHelpers::FObjectFinder<USoundCue> CheckpointSoundCue(TEXT("'/Game/Sound/SFX/other/sfx_Checkpoint'"));
	CheckpointSoundAudioCue = CheckpointSoundCue.Object;
	CheckpointSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CheckpointSoundAudioComp"));
	CheckpointSoundAudioComponent->bAutoActivate = false;
	CheckpointSoundAudioComponent->SetupAttachment(RootComponent);

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCheckpoint::OnOverlap);

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
}

void AMyProjectCheckpoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	

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
			UE_LOG(LogTemp, Warning, TEXT("success"));
			this->hasUsed = true;
			hittedplayer->SetRespawn(vector, rotator);
			OnCheckpoint();
			NextCheckpoint();

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
