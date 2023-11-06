// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/DeployableItem.h"
#include "LockDeployableItem.generated.h"

/**
 * 
 */
UCLASS()
class ALockDeployableItem : public ADeployableItem
{
	GENERATED_BODY()

protected:
	virtual FTransform GetPlacementTransform(bool& OutValidSpawn) override;

};
