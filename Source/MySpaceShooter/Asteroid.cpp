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
    
    life = FMath::RandRange(1, 5);
    baseLife = life;
    
    UE_LOG(LogTemp, Warning, TEXT("Life is %d"), life);

    Spaceship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpaceShip::StaticClass()));

    if (Spaceship && ProjectileMovementComponent)
    {
        float RandomSpeed = FMath::FRandRange(100.f, 400.f);

        ProjectileMovementComponent->InitialSpeed = RandomSpeed;
        ProjectileMovementComponent->MaxSpeed     = RandomSpeed;

        if (Spaceship)
        {
            FVector steering = Seek(Spaceship->GetActorLocation());
            steering.Normalize();
            ProjectileMovementComponent->Velocity = steering * RandomSpeed;
        }
    }

    if (AsteroidMesh)
    {
        UMaterialInterface* BaseMat = AsteroidMesh->GetMaterial(0);
        if (BaseMat)
        {
            DynMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
            AsteroidMesh->SetMaterial(0, DynMaterial);
        }

        FVector RandomScale(
            FMath::FRandRange(1.f, 4.f),
            FMath::FRandRange(1.f, 4.f),
            FMath::FRandRange(1.f, 4.f)
        );
        AsteroidMesh->SetWorldScale3D(RandomScale);
    }

    GameMode = Cast<ASpaceGameMode>(GetWorld()->GetAuthGameMode());
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
            if (Spaceship)
            {
                Spaceship->Score += FMath::RoundToInt(getDestructionScore() * Spaceship->Combo);

                if (Spaceship->Combo < 5.f)
                {
                    Spaceship->Combo += 0.1f;
                }
            }
            
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

int AAsteroid::getDestructionScore()
{
    return ScoreValue * baseLife;
}

void AAsteroid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GameMode && !GameMode->IsInsidePlayArea(GetActorLocation()))
    {
        Destroy();
    }
}