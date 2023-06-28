// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ToolItem.h"
#include "BuildingHammerItem.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ABuildingHammerItem : public AToolItem
{
	GENERATED_BODY()

public:
	ABuildingHammerItem();

	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

	bool GetLookingAtItemDurability(float& OutPercent) const;
};
