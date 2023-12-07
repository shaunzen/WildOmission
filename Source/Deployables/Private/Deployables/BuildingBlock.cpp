// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/BuildingBlock.h"

ABuildingBlock::ABuildingBlock()
{
	MaterialType = EToolType::WOOD;
	UpgradedTier = nullptr;
}

void ABuildingBlock::Upgrade()
{
	if (UpgradedTier == nullptr)
	{
		return;
	}

	ABuildingBlock* SpawnedBuildingBlock = GetWorld()->SpawnActor<ABuildingBlock>(UpgradedTier, this->GetActorLocation(), this->GetActorRotation());
	if (SpawnedBuildingBlock == nullptr)
	{
		return;
	}

	SpawnedBuildingBlock->OnSpawn();

	this->Destroy();
}

bool ABuildingBlock::IsUpgradable() const
{
	return UpgradedTier != nullptr;
}

TEnumAsByte<EToolType> ABuildingBlock::GetMaterialType()
{
	return MaterialType;
}

ABuildingBlock* ABuildingBlock::GetUpgradeDefaultClass() const
{
	if (UpgradedTier == nullptr)
	{
		return nullptr;
	}

	return UpgradedTier.GetDefaultObject();
}
