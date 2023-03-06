// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "WildOmission/Core/Enums/ToolType.h"
#include "ToolItem.generated.h"

UCLASS()
class WILDOMISSION_API AToolItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AToolItem();

	//TODO load the durability from the item
	
	// Called when the item is equiped into the players hands
	virtual void Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	// TODO save the durability to the item

	// Called before the item is unequiped
	virtual void OnUnequip() override;

	//TODO save durability everytime we take damage?
	virtual void Primary() override;

	virtual void Secondary() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> ToolType;
	
	UPROPERTY(EditDefaultsOnly)
	float GatherMultiplyer;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectiveRangeCentimeters;
	
	UPROPERTY(EditDefaultsOnly)
	float SwingTimeSeconds;

	int32 Durability;

	UFUNCTION()
	void ApplyDamage();

private:
	FInventoryItem* FindInInventory();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlaySwingAnimation();

};