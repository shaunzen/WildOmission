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
	
	void Swing();

	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* SwingMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* SwingItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float DamageMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float EffectiveRangeCentimeters;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float SwingSpeedRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool DealDamageToActors;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCameraShakeBase> SwingCameraShake;

	virtual void OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance);

	UFUNCTION()
	void DecrementDurability();

	void PlayImpactSound(const FHitResult& HitResult);
	void SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector);
	void SpawnImpactDecal(const FHitResult& HitResult);
	
	void PlayCameraShake();

};
