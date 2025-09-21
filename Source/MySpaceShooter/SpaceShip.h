// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SpaceGameMode.h"
#include "SpaceShip.generated.h"

UCLASS()
class MYSPACESHOOTER_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ASpaceGameMode* GameMode;
	
	UPROPERTY(BlueprintReadWrite, Category="Movement")
	FVector LastMoveDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	class UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	class UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* FireAction;

	UFUNCTION()
	void OnOverlapWithAsteroid(UPrimitiveComponent* OverlappedComponent,
							   AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex,
							   bool bFromSweep,
							   const FHitResult& SweepResult);
private:
	void MoveForward(const FInputActionValue& Value);
	
	void MoveRight(const FInputActionValue& Value);

	void FireMissile();
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<class AMissile> MissileClass;

	UPROPERTY(EditAnywhere, Category="Combat")
	float FireRate = 0.2f; // 0.2s entre chaque tir

	float LastFireTime = -100.f;

	int life = 3;
};
