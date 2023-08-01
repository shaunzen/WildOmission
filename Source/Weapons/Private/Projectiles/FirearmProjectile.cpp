// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/FirearmProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AFirearmProjectile::AFirearmProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 5000.0f;
	MovementComponent->MaxSpeed = 5000.0f;
}

// Called when the game starts or when spawned
void AFirearmProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirearmProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

