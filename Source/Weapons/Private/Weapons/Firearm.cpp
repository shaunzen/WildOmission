// Copyright Telephone Studios. All Rights Reserved.


#include "Weapons/Firearm.h"
#include "Components/EquipComponent.h"
#include "Projectiles/FirearmProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	ProjectileClass = nullptr;
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

	// Find Spawn location for projectile
	const FVector OwnerForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetOwnerEquipComponent()->GetOwnerControlRotation().Yaw, 0.0f));
	const FVector ProjectileSpawnLocation = GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 70.0f) + OwnerForwardVector * 50.0f;
	
	// Spawn the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetOwnerPawn();
	SpawnParams.Owner = GetOwner();
	GetWorld()->SpawnActor<AFirearmProjectile>(ProjectileClass, ProjectileSpawnLocation, GetOwnerEquipComponent()->GetOwnerControlRotation(), SpawnParams);
}

void AFirearm::Multi_PlayFireSound_Implementation()
{
	if (GetOwner() == nullptr || FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
}
