// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/DeployableItemBase.h"
#include "LockDeployableItem.generated.h"

/**
 * 
 */
UCLASS()
class ALockDeployableItem : public ADeployableItemBase
{
	GENERATED_BODY()

protected:
	virtual void OnPlace() override;
	virtual bool GetPlacementTransform(FTransform& OutPlacementTransform) override;

};
