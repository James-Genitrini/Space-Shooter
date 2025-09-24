// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

UCLASS()
class MYSPACESHOOTER_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AActor> AsteroidClass;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float MinX = -2100.f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float MaxX = 1000.f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float MinY = -2200.f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float MaxY = 2600.f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	float SpawnInterval = 2.f;

	FTimerHandle SpawnTimer;

	FVector GetRandomEdgePoint() const;
	void SpawnAsteroid();

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
