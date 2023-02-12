// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryItem.h"

bool FInventoryItem::CompareNames(const FInventoryItem& Item, const FName& ItemName)
{
	return Item.Name == ItemName;
}

void FInventoryItem::Set(const FInventoryItem& InItem)
{
	Name = InItem.Name;
	Quantity = InItem.Quantity;
}

void FInventoryItem::Set(const FName& InName, const int32& InQuantity)
{
	Name = InName;
	Quantity = InQuantity;
}

bool FInventoryItem::IsZero() const
{
	return Quantity == 0;
}

void FInventoryItem::Clear()
{
	Name = FName("");
	Quantity = 0;
}