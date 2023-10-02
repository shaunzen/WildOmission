// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "Enums/ToolType.h"
#include "Structs/InventoryItem.h"
#include "ToolItem.generated.h"

class UNiagaraSystem;

UCLASS()
class EQUIPABLEITEMS_API AToolItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AToolItem();

	// Called when the item is equiped into the players hands
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	// Called before the item is unequiped
	virtual void OnUnequip() override;

	virtual void OnPrimaryHeld() override;

	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

	float GetGatherMultiplier() const;
	
	float GetSwingSpeedRate() const;

	bool IsQualityTool() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TEnumAsByte<EToolType> ToolType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float GatherMultiplier;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float EffectiveRangeCentimeters;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float SwingSpeedRate;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool QualityTool;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCameraShakeBase> SwingCameraShake;

	UPROPERTY()
	int32 Durability;

	UFUNCTION()
	void UpdateDurability();
	
	void PlayImpactSound(const FHitResult& HitResult);
	void SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector);
	void SpawnImpactDecal(const FHitResult& HitResult);

	void PlayCameraShake();

	FInventoryItem* FindInInventory();

};