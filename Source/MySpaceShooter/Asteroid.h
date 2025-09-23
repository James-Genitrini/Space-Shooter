// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceShip.h"
#include "GameFramework/Actor.h"
#include "SpaceGameMode.h"
#include "Asteroid.generated.h"

UCLASS()
class MYSPACESHOOTER_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ASpaceGameMode* GameMode;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* AsteroidMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	void TakeDamageFromMissile();

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial;

	void ResetColor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay")
	int ScoreValue = 100;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay")
	int baseLife;
	
private:
	int life;

	FVector Seek(const FVector& TargetPosition);

	ASpaceShip* Spaceship;

	int getDestructionScore();
	
	// UFUNCTION()
	// void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
