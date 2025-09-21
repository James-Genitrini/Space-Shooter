#include "Asteroid.h"
#include "SpaceGameMode.h"
#include "SpaceShip.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

// Sets default values
AAsteroid::AAsteroid()
{
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
    AsteroidMesh->SetupAttachment(Root);

    AsteroidMesh->SetGenerateOverlapEvents(true);
    AsteroidMesh->SetNotifyRigidBodyCollision(true);
    AsteroidMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
    AsteroidMesh->SetCollisionResponseToAllChannels(ECR_Block);
    AsteroidMesh->SetCollisionObjectType(ECC_WorldDynamic);
    AsteroidMesh->SetSimulatePhysics(false);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 150.f;
    ProjectileMovementComponent->MaxSpeed = 150.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAsteroid::BeginPlay()
{
    Super::BeginPlay();

    GameMode = Cast<ASpaceGameMode>(GetWorld()->GetAuthGameMode());

    life = FMath::RandRange(1, 5);
    UE_LOG(LogTemp, Warning, TEXT("Life is %d"), life);

    Spaceship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpaceShip::StaticClass()));

    if (Spaceship && ProjectileMovementComponent)
    {
        FVector steering = Seek(Spaceship->GetActorLocation());
        ProjectileMovementComponent->Velocity = steering;
    }

    if (AsteroidMesh)
    {
        UMaterialInterface* BaseMat = AsteroidMesh->GetMaterial(0);
        if (BaseMat)
        {
            DynMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
            AsteroidMesh->SetMaterial(0, DynMaterial);
        }
    }
}

FVector AAsteroid::Seek(const FVector& TargetPosition)
{
    FVector vDesiredLocation = TargetPosition - GetActorLocation();
    vDesiredLocation.Normalize();
    vDesiredLocation *= ProjectileMovementComponent->MaxSpeed;
    return vDesiredLocation;
}

void AAsteroid::TakeDamageFromMissile()
{
    if (life > 0)
    {
        life--;

        if (DynMaterial)
        {
            DynMaterial->SetVectorParameterValue("TintColor", FLinearColor::Red);

            FTimerHandle Timer;
            GetWorldTimerManager().SetTimer(
                Timer,
                this,
                &AAsteroid::ResetColor,
                0.2f,
                false
            );
        }

        if (life == 0)
        {
            Destroy();
        }
    }
}

void AAsteroid::ResetColor()
{
    if (DynMaterial)
    {
        DynMaterial->SetVectorParameterValue("TintColor", FLinearColor::White);
    }
}

void AAsteroid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GameMode && !GameMode->IsInsidePlayArea(GetActorLocation()))
    {
        Destroy();
    }
}