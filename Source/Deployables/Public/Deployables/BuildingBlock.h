// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "BuildingBlock.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ABuildingBlock : public ADeployable
{
	GENERATED_BODY()
	
public:
	ABuildingBlock();
	void Upgrade();
	bool IsUpgradable() const;

	TEnumAsByte<EToolType> GetMaterialType();

	ABuildingBlock* GetUpgradeDefaultClass() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Deployable")
	TEnumAsByte<EToolType> MaterialType;

	UPROPERTY(EditDefaultsOnly, Category = "Deployable")
	TSubclassOf<ABuildingBlock> UpgradedTier;
};
