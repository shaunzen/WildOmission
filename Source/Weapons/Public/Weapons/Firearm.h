// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Firearm.generated.h"

class AFirearmProjectile;
class UPlayerInventoryComponent;

UCLASS()
class WEAPONS_API AFirearm : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AFirearm();
	
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	virtual void OnPrimaryPressed() override;
	virtual void OnReloadPressed() override;

	virtual void OnReloadAnimationClimax(bool FromFirstPersonInstance);

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFirearmProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FName AmmoItemID;

	UPROPERTY()
	int32 CurrentAmmo;
	UPROPERTY()
	int32 MaxAmmo;
	UPROPERTY()
	int32 Durability;
	
	void FireProjectile();

	void RetrieveInventoryStats();
	void UpdateInventoryStats();
	UPlayerInventoryComponent* GetOwningPlayerInventory() const;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayFireSound();

};
