// Copyright Telephone Studios. All Rights Reserved.


#include "InventorySlot.h"

void FInventorySlot::SetItem(const FInventoryItem& InItem)
{
	Item = InItem;
}

void FInventorySlot::ClearItem()
{
	Item.Clear();
}

bool FInventorySlot::CompareItem(const FInventoryItem& ItemToCompare) const
{
	return Item.Name == ItemToCompare.Name;
}

bool FInventorySlot::IsEmpty() const
{
	return Item.Quantity == 0 || Item.Name == FName("");
}