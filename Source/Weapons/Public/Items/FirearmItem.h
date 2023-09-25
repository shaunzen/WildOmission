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
	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;
	virtual void OnSecondaryPressed() override;
	virtual void OnSecondaryReleased() override;
	virtual void OnReloadPressed() override;
	virtual void OnReloadAnimationClimax(bool FromFirstPersonInstance) override;

	int32 GetRateOfFire() const;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* MuzzleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 RateOfFireRPM;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UNiagaraSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCameraShakeBase> ReloadStartCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCameraShakeBase> ReloadClimaxCameraShake;

	virtual void PlayFireEffects() override;
	void PlayMuzzleFlash();
	void InvokeFireCooldownTimer();
	
	void PlayFireCameraShake();
	void PlayReloadStartCameraShake();
	void PlayReloadClimaxCameraShake();

	bool CanFire;

	UFUNCTION()
	void ResetCanFire();

	virtual void Fire();

};
