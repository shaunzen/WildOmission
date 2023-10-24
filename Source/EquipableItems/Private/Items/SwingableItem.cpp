// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SwingableItem.h"
#include "Log.h"

ASwingableItem::ASwingableItem()
{
}

void ASwingableItem::DecrementDurability()
{
}

void ASwingableItem::PlayImpactSound(const FHitResult& HitResult)
{
	if (HitResult.PhysMaterial == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to get physical material."));
		return;
	}

	USoundBase* ImpactSound = USurfaceHelpers::GetImpactSound(HitResult.PhysMaterial.Get()->SurfaceType);
	if (ImpactSound == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to get impact sound."));
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
}

void ASwingableItem::SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector)
{
	if (HitResult.PhysMaterial == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to get physical material."));
		return;
	}

	UNiagaraSystem* ImpactParticles = USurfaceHelpers::GetImpactParticles(HitResult.PhysMaterial->SurfaceType);
	if (ImpactParticles == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to spawn impact effects, Impact Effects nullptr."));
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, HitResult.ImpactPoint, (-ImpactorForwardVector).Rotation());
}

void ASwingableItem::SpawnImpactDecal(const FHitResult& HitResult)
{
	if (HitResult.PhysMaterial == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Cannot spawn impact decal, Failed to get physical material."));
		return;
	}

	UMaterialInterface* DecalMaterial = USurfaceHelpers::GetImpactDecal(HitResult.PhysMaterial->SurfaceType);
	if (DecalMaterial == nullptr)
	{
		return;
	}

	FVector DecalSize = FVector(8.0f, 15.0f, 15.0f);
	UGameplayStatics::SpawnDecalAttached(DecalMaterial, DecalSize, HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 120.0f);

}

void ASwingableItem::PlayCameraShake()
{
	if (SwingCameraShake == nullptr || GetOwnerPawn() == nullptr || !GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr || UserSettings->GetCameraShakeEnabled() == false)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwnerPawn()->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	OwnerPlayerController->ClientStartCameraShake(SwingCameraShake);
}
