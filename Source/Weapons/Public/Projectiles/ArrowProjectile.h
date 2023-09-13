// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/WeaponProjectile.h"
#include "ArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API AArrowProjectile : public AWeaponProjectile
{
	GENERATED_BODY()
	
public:
	AArrowProjectile();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

};
