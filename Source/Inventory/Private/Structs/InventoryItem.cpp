// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryItem.h"

bool FInventoryItem::CompareNames(const FInventoryItem& Item, const FName& ItemName)
{
	return Item.Name == ItemName;
}

bool FInventoryItem::CompareID(const FInventoryItem& Item, const uint32& ItemID)
{
	return Item.UniqueID == ItemID;
}

void FInventoryItem::Set(const FInventoryItem& InItem)
{
	Name = InItem.Name;
	Quantity = InItem.Quantity;
	UniqueID = InItem.UniqueID;
}

void FInventoryItem::Set(const FName& InName, const int32& InQuantity)
{
	Name = InName;
	Quantity = InQuantity;
	UniqueID = 0;
}

bool FInventoryItem::IsZero() const
{
	return Quantity == 0;
}

void FInventoryItem::Clear()
{
	Name = FName("");
	Quantity = 0;
	UniqueID = 0;
}

int32 FInventoryItem::GetStat(const FName& StatName) const
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

void FInventoryItem::SetStat(const FName& StatName, const int32& Value)
{
	bool StatExists = false;

	if (Stats.Num() == 0)
	{
		AddStat(StatName, Value);
	}

	for (FItemStat& Stat : Stats)
	{
		if (Stat.Name != StatName)
		{
			continue;
		}

		StatExists = true;
		Stat.Value = Value;
		break;
	}

	if (StatExists == false)
	{
		AddStat(StatName, Value);
	}
}

void FInventoryItem::AddStat(const FName& StatName, const int32& Value)
{
	FItemStat NewStat;
	NewStat.Name = StatName;
	NewStat.Value = Value;

	Stats.Add(NewStat);
}