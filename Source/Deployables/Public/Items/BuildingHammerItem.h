// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ToolItem.h"
#include "BuildingHammerItem.generated.h"

class ADeployable;

UCLASS()
class DEPLOYABLES_API ABuildingHammerItem : public AToolItem
{
	GENERATED_BODY()

public:
	ABuildingHammerItem();

	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

	bool GetLookingAtItemDurability(float& OutCurrentDurability, float& OutMaxDurability, FString& OutActorName) const;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxRepairAmount;

	void AttemptDeployableRepair(ADeployable* DeployableToRepair, const FHitResult& HitResult, const FVector& DirectionVector);
	FInventoryItem GetBaseRepairCostForDeployable(ADeployable* DeployableToRepair) const;

};
