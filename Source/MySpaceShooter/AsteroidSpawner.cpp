// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		&AAsteroidSpawner::SpawnAsteroid,
		SpawnInterval,
		true
	);
}

// Called every frame
// void AAsteroidSpawner::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

FVector AAsteroidSpawner::GetRandomEdgePoint() const
{
	int32 Side = FMath::RandRange(0, 3);
	switch (Side)
	{
		case 0:
			return FVector(MinX, FMath::FRandRange(MinY, MaxY), GetActorLocation().Z);
		case 1:
			return FVector(MaxX, FMath::FRandRange(MinY, MaxY), GetActorLocation().Z);
		case 2:
			return FVector(FMath::FRandRange(MinX, MaxX), MinY, GetActorLocation().Z);
	default:
			return FVector(FMath::FRandRange(MinX, MaxX), MaxY, GetActorLocation().Z);
	}
}

void AAsteroidSpawner::SpawnAsteroid()
{
	FVector SpawnPoint = GetRandomEdgePoint();
	FRotator SpawnRot = FRotator::ZeroRotator;

	GetWorld()->SpawnActor<AActor>(AsteroidClass, SpawnPoint, SpawnRot);
}