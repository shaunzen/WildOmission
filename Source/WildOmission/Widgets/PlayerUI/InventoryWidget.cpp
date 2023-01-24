// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/CanvasPanel.h"
#include "WildOmission/Components/InventoryComponent.h"

void UInventoryWidget::SetSelectedItemWidget(USelectedItemWidget* InWidget)
{
	SelectedItemWidget = InWidget;
	SelectedItemWidget->Hide();
}

void UInventoryWidget::SetComponent(UInventoryComponent* InInventoryComponent)
{
	// Check if inventory and slot are valid pointers
	if (InInventoryComponent == nullptr || InventorySlotWidgetClass == nullptr)
	{
		return;
	}
	
	InventoryComponent = InInventoryComponent;

	CreateInventorySlots();

	// Set default visibility
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryWrapBox->SetVisibility(ESlateVisibility::Hidden);
	ToolbarWrapBox->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryWidget::CreateInventorySlots()
{
	// Add Inventory slots to ui
	for (int32 i = 0; i < 30; ++i)
	{
		// Create new slot widget
		UInventorySlotWidget* NewInventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		NewInventorySlot->Setup(this);
		if (i < 6)
		{
			ToolbarWrapBox->AddChild(NewInventorySlot);
		}
		else
		{
			InventoryWrapBox->AddChild(NewInventorySlot);
		}
		// Add this slot to slot array
		InventorySlots.Add(NewInventorySlot);
	}
}

bool UInventoryWidget::AddItem(FName ItemName, int32 Quantity, int32& Remaining)
{
	if (InventoryComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidget: InventoryComponent was nullptr"));
		return false;
	}

	int32 QuantityToAdd = Quantity;
	FItem* ItemData = InventoryComponent->GetItemData(ItemName);
	if (ItemData == nullptr)
	{
		return false;
	}
	
	// Check for a slot that already has this item, If not than try to find a slot that is empty
	if (!AddItemToPopulatedSlot(ItemName, ItemData, QuantityToAdd))
	{
		AddItemToEmptySlot(ItemName, ItemData, QuantityToAdd);
	}

	Remaining = QuantityToAdd;
	return QuantityToAdd == 0;
}

bool UInventoryWidget::RemoveItem(FName ItemName, int32 Quantity, int32& Remaining)
{
	Remaining = Quantity;
	// Loop through all slots
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		// if this slot doesnt have the item we are looking for move to the next
		if (InventorySlot->GetCurrentItem()->Name != ItemName)
		{
			continue;
		}

		// If we are trying to remove more than is currently in this slot
		if (InventorySlot->GetCurrentItem()->Quantity <= Remaining)
		{
			Remaining -= InventorySlot->GetCurrentItem()->Quantity;
			InventorySlot->ClearItem();
			continue;
		}
		else
		{
			// Remove just the amount we need from this slot
			FSlotItem NewSlotItem = *InventorySlot->GetCurrentItem();
			NewSlotItem.Quantity -= Remaining;
			InventorySlot->SetItem(NewSlotItem);
			Remaining = 0;
		}

		// Break if there is no more left to remove
		if (Remaining == 0)
		{
			break;
		}

	}
	return Remaining == 0;
}

bool UInventoryWidget::AddItemToPopulatedSlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
{
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}
		if (InventorySlot->IsFull() || InventorySlot->GetCurrentItem()->Name != ItemName)
		{
			continue;
		}
		
		if (InventorySlot->GetCurrentItem()->Quantity + QuantityToAdd > ItemData->StackSize)
		{
			QuantityToAdd -= ItemData->StackSize - InventorySlot->GetCurrentItem()->Quantity;
			FSlotItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = ItemData->StackSize;
			InventorySlot->SetItem(NewSlotItem);
		}
		else
		{
			FSlotItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = QuantityToAdd + InventorySlot->GetCurrentItem()->Quantity;
			InventorySlot->SetItem(NewSlotItem);
			QuantityToAdd = 0;
		}
	}
	return QuantityToAdd == 0;
}

bool UInventoryWidget::AddItemToEmptySlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
{
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}
		if (InventorySlot->IsFull() || InventorySlot->GetCurrentItem()->Quantity > 0)
		{
			continue;
		}

		if (QuantityToAdd > ItemData->StackSize)
		{
			QuantityToAdd -= ItemData->StackSize;
			FSlotItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = ItemData->StackSize;
			InventorySlot->SetItem(NewSlotItem);
		}
		else
		{
			FSlotItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = QuantityToAdd;
			InventorySlot->SetItem(NewSlotItem);
			QuantityToAdd = 0;
		}
	}

	return QuantityToAdd == 0;
}

void UInventoryWidget::Open()
{
	InventoryName->SetVisibility(ESlateVisibility::Visible);
	InventoryWrapBox->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryWidget::Close()
{
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryWrapBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::StartDragging(FSlotItem Item)
{
	FItem* ItemData = InventoryComponent->GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Item ID: %s"), *Item.Name.ToString());
		return;
	}
	SelectedItem.Set(Item.Name, Item.Quantity);
	SelectedItemWidget->SetItem(ItemData->Thumbnail, SelectedItem.Quantity);
	SelectedItemWidget->Show();
	bCurrentlyDragging = true;
}

void UInventoryWidget::EndDragging()
{
	SelectedItem.Name = FName("");
	SelectedItem.Quantity = 0;
	SelectedItemWidget->Hide();
	bCurrentlyDragging = false;
}

bool UInventoryWidget::Dragging() const
{
	return bCurrentlyDragging;
}

void UInventoryWidget::DropSelectedItem(bool bDropAll)
{
	if (bDropAll == true)
	{
		// Remove all the selected from our inventory list
		InventoryComponent->RemoveItem(SelectedItem.Name, SelectedItem.Quantity, true);

		EndDragging();
	}
	else
	{
		// Remove a single item of the selected
		InventoryComponent->RemoveItem(SelectedItem.Name, 1, true);
		
		// Calculate the new selection
		FSlotItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;

		// Stop dragging if there is no more selected item
		if (NewSelection.Quantity <= 0)
		{
			EndDragging();
		}
		else
		{
			// Update the selection
			StartDragging(NewSelection);
		}
	}
}

FSlotItem* UInventoryWidget::GetSelectedItem()
{
	return &SelectedItem;
}

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}