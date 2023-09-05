// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/BuildingBlock.h"

ABuildingBlock::ABuildingBlock()
{
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
