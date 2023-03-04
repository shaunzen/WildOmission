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