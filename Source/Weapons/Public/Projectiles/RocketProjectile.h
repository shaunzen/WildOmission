// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/WeaponProjectile.h"
#include "RocketProjectile.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API ARocketProjectile : public AWeaponProjectile
{
	GENERATED_BODY()
	
public:
	ARocketProjectile();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ExplosionSystem;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayExplosionEffects();

};
