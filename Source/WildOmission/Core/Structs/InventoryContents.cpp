// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryContents.h"

int32 FInventoryContents::GetItemQuantity(const FName& ItemName)
{
	int32 Index = GetItemIndex(ItemName);
	if (Index == INDEX_NONE)
	{
		return 0;
	}
	return Contents[Index].Quantity;
}

bool FInventoryContents::HasItem(const FName& ItemName)
{
	return Contents.FindByPredicate([&ItemName](const FInventoryItem& ItemData) {
		return ItemData.CompareNames(ItemData, ItemName);
		}) != nullptr;
}

void FInventoryContents::AddItem(const FName& ItemName, const int32& QuantityToAdd)
{
	if (HasItem(ItemName))
	{
		int32 ItemIndex = GetItemIndex(ItemName);
		Contents[ItemIndex].Quantity += QuantityToAdd;
	}
	else
	{
		FInventoryItem NewItem;
		NewItem.Name = ItemName;
		NewItem.Quantity = QuantityToAdd;
		Contents.Add(NewItem);
	}
}

void FInventoryContents::RemoveItem(const FName& ItemName, const int32& QuantityToRemove)
{
	if (!HasItem(ItemName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot remove %s, item does not exist in inventory."), *ItemName.ToString());
		return;
	}

	int32 ItemIndex = GetItemIndex(ItemName);
	Contents[ItemIndex].Quantity -= QuantityToRemove;

	if (Contents[ItemIndex].Quantity <= 0)
	{
		Contents.RemoveAt(ItemIndex);
	}
}

int32 FInventoryContents::GetItemIndex(const FName& ItemName)
{
	return Contents.IndexOfByPredicate([&ItemName](const FInventoryItem& ItemData) {
		return ItemData.CompareNames(ItemData, ItemName);
		});
}