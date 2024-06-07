// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ManualActionFirearmItem.h"
#include "ManualShotgunFirearmItem.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API AManualShotgunFirearmItem : public AManualActionFirearmItem
{
	GENERATED_BODY()

public:
	AManualShotgunFirearmItem();

protected:
	virtual void PlayFireEffects() override;

};
