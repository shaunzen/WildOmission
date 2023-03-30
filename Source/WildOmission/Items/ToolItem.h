// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "WildOmission/Core/Enums/ToolType.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "ToolItem.generated.h"

UCLASS()
class WILDOMISSION_API AToolItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AToolItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called when the item is equiped into the players hands
	virtual void Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	// Called before the item is unequiped
	virtual void OnUnequip() override;

	virtual void Primary() override;

	virtual void Secondary() override;

	virtual void Harvest();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetPrimaryMontage() const;

	float GetGatherMultiplier() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> ToolType;
	
	UPROPERTY(EditDefaultsOnly)
	float GatherMultiplier;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectiveRangeCentimeters;
	
	UPROPERTY(EditDefaultsOnly)
	float SwingTimeSeconds;

	UPROPERTY(Replicated)
	int32 Durability;

	UFUNCTION()
	void ApplyDamage();
	
	FInventoryItem* FindInInventory();

private:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* PrimaryMontage;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* HarvestSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayPrimaryMontage();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayHarvestSound();
};