// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/DeployableItemBase.h"
#include "DeployableItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeployablePlacedSignature, ADeployable*, DeployablePlaced);

UCLASS()
class DEPLOYABLES_API ADeployableItem : public ADeployableItemBase
{
	GENERATED_BODY()
	
public:
	FOnDeployablePlacedSignature OnDeployablePlaced;

protected:
	virtual void OnPlace() override;

	virtual bool GetPlacementTransform(FTransform& OutPlacementTransform) override;
	float GetOffsetFromNearestSnapDegree(const float& InAxis) const;

};
