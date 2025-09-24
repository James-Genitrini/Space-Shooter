// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"

#include "Asteroid.h"
#include "Missile.h"
#include "SpaceGameMode.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;

	ShipMesh->SetGenerateOverlapEvents(true);
	ShipMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // ou un profil custom qui bloque l'astéroïde en overlap
	ShipMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlapWithAsteroid);

	// Mouvement
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 1500.f;
	MovementComponent->Acceleration = 4000.f;
	MovementComponent->Deceleration = 4000.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ASpaceGameMode>(GetWorld()->GetAuthGameMode());

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASpaceShip::MoveForward);
		EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASpaceShip::MoveRight);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &ASpaceShip::FireMissile);
	}


}

void ASpaceShip::MoveForward(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	if (AxisValue != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), AxisValue);

		LastMoveDirection += GetActorForwardVector() * AxisValue; // Permet de savoir où vise le pawn pour le missile
	}
}

void ASpaceShip::MoveRight(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	if (AxisValue != 0.f)
	{
		AddMovementInput(GetActorRightVector(), AxisValue);

		LastMoveDirection += GetActorRightVector() * AxisValue;
	}
}

void ASpaceShip::FireMissile()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Vérifie le cooldown
	if (CurrentTime - LastFireTime < FireRate)
	{
		return;
	}

	if (LastMoveDirection.IsNearlyZero())
	{
		return;
	}

	FVector SpawnLocation = GetActorLocation() + LastMoveDirection * 100.f;
	FRotator SpawnRotation = LastMoveDirection.GetSafeNormal().Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AMissile>(MissileClass, SpawnLocation, SpawnRotation, SpawnParams);

	LastFireTime = CurrentTime;
}

void ASpaceShip::OnOverlapWithAsteroid(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AAsteroid* HitAsteroid = Cast<AAsteroid>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpaceShip overlaps Asteroid!"));

		life--;

		if (life <= 0)
		{
			if (GameMode)
			{
				GameMode->ShowGameOver();
			}

			ShipMesh->SetVisibility(false, true);
			SetActorEnableCollision(false);
			DisableInput(Cast<APlayerController>(GetController()));
		}
		
		HitAsteroid->Destroy();

		Combo = 1.f;
	}
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// On normalise pour n'avoir que la direction
	if (!LastMoveDirection.IsNearlyZero())
	{
		LastMoveDirection = LastMoveDirection.GetSafeNormal();
	}

	if (GameMode)
	{
		FVector Location = GetActorLocation();
		Location.X = FMath::Clamp(Location.X, GameMode->MinXPawn, GameMode->MaxXPawn);
		Location.Y = FMath::Clamp(Location.Y, GameMode->MinYPawn, GameMode->MaxYPawn);
		SetActorLocation(Location);
	}
}