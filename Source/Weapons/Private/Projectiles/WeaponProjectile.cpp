// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/WeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectiles/CollectableProjectile.h"
#include "Engine/DamageEvents.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SurfaceHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);

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
	MovementComponent->bRotationFollowsVelocity = true;

	Damage = 15.0f;
	Velocity = 50000.0f;
	CollectableClass = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundBase> HitMarkerSoundObject(TEXT("/Game/Weapons/Audio/A_Hitmarker_Body_02"));
	if (HitMarkerSoundObject.Succeeded())
	{
		HitMarkerSound = HitMarkerSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> HitMarkerHeadshotSoundObject(TEXT("/Game/Weapons/Audio/A_Hitmarker_Head"));
	if (HitMarkerHeadshotSoundObject.Succeeded())
	{
		HitMarkerHeadshotSound = HitMarkerHeadshotSoundObject.Object;
	}
}

void AWeaponProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponProjectile, Velocity);
}

void AWeaponProjectile::Setup(const FName& ItemID)
{
	FItemData* ItemData = UInventoryComponent::GetItemData(ItemID);
	if (ItemData == nullptr)
	{
		return;
	}

	int32 DamageStat = ItemData->GetStat(TEXT("Damage"));
	if (DamageStat != -1)
	{
		Damage = DamageStat;
	}

	int32 VelocityStat = ItemData->GetStat(TEXT("Velocity"));
	if (VelocityStat != -1)
	{
		Velocity = VelocityStat;
	}
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	
	if (!HasAuthority())
	{
		return;
	}
	
	CollisionComponent->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnHit);
}

void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	// Spawn impact effects
	Multi_SpawnImpactEffects(Hit);

	// Deal Damage
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (HitPawn)
	{
		const bool Headshot = Hit.BoneName == TEXT("Head");
		const float HeadshotMultiplier = 10.0f * Headshot;
		const float TotalDamage = Damage + HeadshotMultiplier;

		Headshot ? Client_PlayHitMarkerHeadshotSound() : Client_PlayHitMarkerSound();
		
		UE_LOG(LogTemp, Warning, TEXT("Damage %f, With Headshot Multiplier %f"), Damage, TotalDamage);

		FPointDamageEvent HitByProjectileEvent(TotalDamage, Hit, NormalImpulse, nullptr);
		HitPawn->TakeDamage(TotalDamage, HitByProjectileEvent, GetInstigatorController<AController>(), this);
	}

	bool ProjectileIntact = UKismetMathLibrary::RandomBoolWithWeight(0.75f);
	if (ProjectileIntact && CollectableClass)
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

void AWeaponProjectile::OnRep_Velocity()
{
	MovementComponent->InitialSpeed = Velocity;
	MovementComponent->MaxSpeed = Velocity;
}

void AWeaponProjectile::Client_PlayHitMarkerSound_Implementation()
{
	if (HitMarkerSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), HitMarkerSound);
}

void AWeaponProjectile::Client_PlayHitMarkerHeadshotSound_Implementation()
{
	if (HitMarkerHeadshotSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), HitMarkerHeadshotSound);
}

void AWeaponProjectile::Multi_SpawnImpactEffects_Implementation(const FHitResult& HitResult)
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