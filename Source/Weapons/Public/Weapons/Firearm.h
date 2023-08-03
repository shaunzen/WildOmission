// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Firearm.generated.h"

class AFirearmProjectile;

UCLASS()
class WEAPONS_API AFirearm : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AFirearm();
	
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	virtual void OnPrimaryPressed() override;
	virtual void OnReloadPressed() override;


private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFirearmProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FName AmmoItemID;

	int32 CurrentAmmo;
	int32 MaxAmmo;
	
	void FireProjectile();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayFireSound();

};
