// Copyright Telephone Studios. All Rights Reserved.


#include "UI/InventorySlot.h"

void FInventorySlot::SetItem(const FInventoryItem& InItem)
{
	Item = InItem;
}

void FInventorySlot::ClearItem()
{
	Item.Clear();
}

bool FInventorySlot::SameItemNameAs(const FInventoryItem& ItemToCompare) const
{
	return Item.Name == ItemToCompare.Name;
}

bool FInventorySlot::SameItemIdAs(const FInventoryItem& ItemToCompare) const
{
	return Item.UniqueID == ItemToCompare.UniqueID;
}

bool FInventorySlot::IsEmpty() const
{
	return Item.Quantity == 0 || Item.Name == FName("");
}