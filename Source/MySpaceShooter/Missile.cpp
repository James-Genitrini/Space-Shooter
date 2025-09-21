// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

#include "Asteroid.h"
#include "SpaceGameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	MissileMesh->SetupAttachment(RootComponent);
	MissileMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	MissileMesh->SetGenerateOverlapEvents(true);
	MissileMesh->SetNotifyRigidBodyCollision(true);
	MissileMesh->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	MissileMesh->SetCollisionResponseToAllChannels(ECR_Block);
	MissileMesh->SetCollisionObjectType(ECC_WorldDynamic);
	MissileMesh->SetSimulatePhysics(false);
	MissileMesh->OnComponentHit.AddDynamic(this, &AMissile::OnHit);
	MissileMesh->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f; // Top-down no gravity for this
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ASpaceGameMode>(GetWorld()->GetAuthGameMode());
}


void AMissile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		Destroy();
	}
}

void AMissile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AAsteroid* HitAsteroid = Cast<AAsteroid>(OtherActor))
	{
		HitAsteroid->TakeDamageFromMissile();

		Destroy();
	}
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameMode && !GameMode->IsInsidePlayArea(GetActorLocation()))
	{
		Destroy();
	}
}