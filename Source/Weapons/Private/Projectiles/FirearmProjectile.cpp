// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/FirearmProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SurfaceHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "Log.h"

// Sets default values
AFirearmProjectile::AFirearmProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->bTraceComplexOnMove = true;
	CollisionComponent->bReturnMaterialOnMove = true;
	RootComponent = CollisionComponent;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 50000.0f;
	MovementComponent->MaxSpeed = 50000.0f;

	Damage = 15.0f;
}

// Called when the game starts or when spawned
void AFirearmProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFirearmProjectile::OnHit);
}

void AFirearmProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

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
	const UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
	if (PhysMaterial == nullptr)
	{
		UE_LOG(LogWeapons, Warning, TEXT("Failed to spawn impact effects, PhysicalMaterial returned a nullptr."));
		return;
	}
	
	USoundBase* ImpactSound = USurfaceHelpers::GetImpactSound(PhysMaterial->SurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
	}

	UNiagaraSystem* ImpactParticles = USurfaceHelpers::GetImpactParticles(PhysMaterial->SurfaceType);
	if (ImpactParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	}

	UMaterialInterface* ImpactDecalMaterial = USurfaceHelpers::GetImpactDecal(PhysMaterial->SurfaceType);
	if (ImpactDecalMaterial)
	{
		const FVector DecalSize = FVector(8.0f, 8.0f, 8.0f);
		UGameplayStatics::SpawnDecalAttached(ImpactDecalMaterial, DecalSize, HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 120.0f);
	}
}