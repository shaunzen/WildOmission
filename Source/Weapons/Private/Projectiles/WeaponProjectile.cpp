// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/WeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectiles/CollectableProjectile.h"
#include "Engine/DamageEvents.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SurfaceHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Log.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel10);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	CollisionComponent->bTraceComplexOnMove = true;
	CollisionComponent->bReturnMaterialOnMove = true;
	RootComponent = CollisionComponent;

	TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailComponent"));
	TrailComponent->SetupAttachment(CollisionComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 50000.0f;
	MovementComponent->MaxSpeed = 50000.0f;

	Damage = 15.0f;
	CollectableClass = nullptr;
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnHit);
}

void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	// Spawn impact effects
	SpawnImpactEffects(Hit);

	// Deal Damage
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (GetOwner()->HasAuthority() && HitPawn)
	{
		FPointDamageEvent HitByProjectileEvent(Damage, Hit, NormalImpulse, nullptr);
		HitPawn->TakeDamage(Damage, HitByProjectileEvent, GetInstigatorController<AController>(), this);
	}

	if (GetOwner()->HasAuthority() && CollectableClass)
	{
		ACollectableProjectile* SpawnedCollectable = GetWorld()->SpawnActor<ACollectableProjectile>(CollectableClass, GetActorLocation(), GetActorRotation());
		if (SpawnedCollectable == nullptr)
		{
			return;
		}

		SpawnedCollectable->AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
	}	

	// Destroy self
	Destroy();
}

void AWeaponProjectile::SpawnImpactEffects(const FHitResult& HitResult)
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