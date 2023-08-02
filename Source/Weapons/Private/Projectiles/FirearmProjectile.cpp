// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/FirearmProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Log.h"

// Sets default values
AFirearmProjectile::AFirearmProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 50000.0f;
	MovementComponent->MaxSpeed = 50000.0f;

	Damage = 15.0f;
}

// Called when the game starts or when spawned
void AFirearmProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentHit.AddDynamic(this, &AFirearmProjectile::OnHit);
}

void AFirearmProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Spawn impact effects
	SpawnImpactEffects(Hit);

	// Deal Damage
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (HasAuthority() && HitPawn)
	{
		FPointDamageEvent HitByProjectileEvent(Damage, Hit, NormalImpulse, nullptr);
		HitPawn->TakeDamage(Damage, HitByProjectileEvent, GetInstigatorController<AController>(), this);
	}

	// Destroy self
	Destroy();
}

void AFirearmProjectile::SpawnImpactEffects(const FHitResult& HitResult)
{
	UE_LOG(LogWeapons, Warning, TEXT("There should be some dust and stuff spawning but I havent added that yet."));
}

// Called every frame
void AFirearmProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}