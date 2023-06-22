// Copyright Telephone Studios. All Rights Reserved.


#include "LootCrateBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "WildOmission/Components/InventoryComponent.h"

ALootCrateBase::ALootCrateBase()
{
	GetInventoryComponent()->SetSlotCount(6);
	ContainerName = TEXT("Loot Crate");
}

void ALootCrateBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority() || GetInventoryComponent()->WasLoadedFromSave())
	{
		return;
	}

	SpawnItems();
}

void ALootCrateBase::SpawnItems()
{
	if (ItemsToDrop.IsEmpty())
	{
		return;
	}

	int32 DropCount = FMath::RandRange(1, GetInventoryComponent()->GetSlotCount());
	for (int32 i = 0; i < DropCount; ++i)
	{
		GetInventoryComponent()->AddItem(GetDrop(), GetOwner());
	}
}

FInventoryItem ALootCrateBase::GetDrop()
{
	FInventoryItem ItemToDrop;
	while (ItemToDrop.Name == FName())
	{
		int32 LootItemIndex =  FMath::RandRange(0, ItemsToDrop.Num() - 1);
		bool WillDrop = UKismetMathLibrary::RandomBoolWithWeight(ItemsToDrop[LootItemIndex].Probability);
		if (WillDrop)
		{
			ItemToDrop = ItemsToDrop[LootItemIndex].Item;
		}
	}

	return ItemToDrop;
}

void ALootCrateBase::OnContainerClosed()
{
	Super::OnContainerClosed();

	if (!GetInventoryComponent()->GetContents()->Contents.IsEmpty())
	{
		return;
	}

	Destroy();
}
