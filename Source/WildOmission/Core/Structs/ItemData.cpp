// Copyright Telephone Studios. All Rights Reserved.


#include "ItemData.h"
#include "ItemStat.h"

FItemData::FItemData()
{
	DisplayName = FName(TEXT("Item"));
	Description = FString(TEXT("This is an Item."));
	Thumbnail = nullptr;
	Mesh = nullptr;
	StackSize = 1000;
	EquipItemClass = nullptr;
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