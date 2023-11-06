// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/DeployableItemBase.h"
#include "DeployableItem.generated.h"

UCLASS()
class DEPLOYABLES_API ADeployableItem : public ADeployableItemBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnPlace() override;

	virtual FTransform GetPlacementTransform(bool& OutValidSpawn) override;
	float GetOffsetFromNearestSnapDegree(const float& InAxis) const;

};
