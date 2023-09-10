// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ProjectileWeaponItem.h"
#include "FirearmItem.generated.h"

class UNiagaraSystem;

UCLASS()
class WEAPONS_API AFirearmItem : public AProjectileWeaponItem
{
	GENERATED_BODY()
	
public:
	AFirearmItem();
	
	virtual void OnPrimaryPressed() override;
	virtual void OnReloadPressed() override;
	virtual void OnReloadAnimationClimax(bool FromFirstPersonInstance) override;

	virtual bool CanFire() const override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* MuzzleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UNiagaraSystem* MuzzleFlashEffect;

	virtual void PlayFireEffects() override;

	void SpawnMuzzleFlash();

	virtual void Fire();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayThirdPersonReloadMontage();

};
