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
	virtual void OnPrimaryPressed() override;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFirearmProjectile> ProjectileClass;

	void FireProjectile();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayFireSound();

};
