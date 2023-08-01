// Copyright Telephone Studios. All Rights Reserved.


#include "Weapons/Firearm.h"
#include "Projectiles/FirearmProjectile.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	ProjectileClass = nullptr;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void AFirearm::OnPrimaryPressed()
{
	FireProjectile();
	Multi_PlayFireSound();
}

void AFirearm::FireProjectile()
{
	if (ProjectileClass == nullptr)
	{
		return;
	}

	// Spawn the projectile
	GetWorld()->SpawnActor<AFirearmProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
}

void AFirearm::Multi_PlayFireSound_Implementation()
{
	if (FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
}
