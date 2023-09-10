// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "ProjectileWeaponItem.generated.h"

class AWeaponProjectile;
class UNiagaraSystem;
class UPlayerInventoryComponent;

UCLASS()
class WEAPONS_API AProjectileWeaponItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AProjectileWeaponItem();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	bool HasAmmo() const;
	virtual bool CanFire() const;

protected:
	UPROPERTY(Replicated)
	int32 CurrentAmmo;
	UPROPERTY(Replicated)
	int32 MaxAmmo;
	UPROPERTY(Replicated)
	int32 Durability;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	USoundBase* OutOfAmmoSound;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AWeaponProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FName AmmoItemID;


	UFUNCTION(NetMulticast, Reliable)
	void Multi_FireEffects();
	virtual void PlayFireEffects();

	void SpawnProjectile();
	void PlayThirdPersonFireAnimation();
	void PlayFireSoundEffect();

	int32 GetRemainingAmmoInInventory() const;
	void RemoveAmmoFromInventory(const int32 AmountToRemove);
	void RetrieveInventoryStats();
	void UpdateInventoryStats();
	

	UPlayerInventoryComponent* GetOwningPlayerInventory() const;

};
