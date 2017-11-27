// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectCheckpoint.h"


// Sets default values
AMyProjectCheckpoint::AMyProjectCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	//CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	//CollisionComp.bGenerateOverlapEvents = true;
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking
	

	// Set as root component
	//RootComponent = CollisionComp;

}

//void AMyProjectCheckpoint::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	AMyProjectCharacter* hittedplayer = Cast<AMyProjectCharacter>(OtherActor);
//	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && hittedplayer != NULL)
//	{
//		
//	}
//}
