// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ProjectileWeaponItem.h"
#include "BowItem.generated.h"


UCLASS()
class WEAPONS_API ABowItem : public AProjectileWeaponItem
{
	GENERATED_BODY()

public:
	ABowItem();
	
	virtual void OnSecondaryPressed() override;
	virtual void OnSecondaryAnimationClimax(bool FromFirstPersonInstance) override;
	virtual void OnSecondaryReleased() override;

private:
	
	bool AtFullDraw;

};
