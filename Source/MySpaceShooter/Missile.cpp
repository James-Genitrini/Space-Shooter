// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

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

	MissileMesh->OnComponentHit.AddDynamic(this, &AMissile::OnHit);
	
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
	
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		Destroy();
	}
}