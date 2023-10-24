// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "SwingableItem.generated.h"

/**
 * 
 */
UCLASS()
class EQUIPABLEITEMS_API ASwingableItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ASwingableItem();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* SwingMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* SwingItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float EffectiveRangeCentimeters;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float SwingSpeedRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCameraShakeBase> SwingCameraShake;

	UFUNCTION()
	void DecrementDurability();

	void PlayImpactSound(const FHitResult& HitResult);
	void SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector);
	void SpawnImpactDecal(const FHitResult& HitResult);
	
	void PlayCameraShake();

};
