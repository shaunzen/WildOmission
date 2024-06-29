// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ItemData.h"
#include "Structs/ItemStat.h"

FItemData::FItemData()
{
	DisplayName = TEXT("Item");
	Thumbnail = nullptr;
	Description = TEXT("This is an Item.");
	Category = EItemCategory::Item;
	Mesh = nullptr;
	StackSize = 1000;
	EquipItemClass = nullptr;
	HiddenInCreativeMode = false;
}

int32 FItemData::GetStat(const FName& StatName)
{
	int32 StatValue = -1;

	if (Stats.Num() == 0)
	{
		return StatValue;
	}

	for (const FItemStat& Stat : Stats)
	{
		if (Stat.Name != StatName)
		{
			continue;
		}

		StatValue = Stat.Value;
		break;
	}

	return StatValue;
}