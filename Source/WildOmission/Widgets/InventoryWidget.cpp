// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/CanvasPanel.h"
#include "../ActorComponents/InventoryComponent.h"

void UInventoryWidget::SetSelectedItemWidget(USelectedItemWidget* InWidget)
{
	SelectedItemWidget = InWidget;
	SelectedItemWidget->Hide();
}

void UInventoryWidget::SetComponent(UInventoryComponent* InInventoryComponent)
{
	// Check for if inventory and slot are valid pointers
	if (InInventoryComponent == nullptr || InventorySlotWidgetClass == nullptr)
	{
		return;
	}
	
	InventoryComponent = InInventoryComponent;

	// Add Inventory slots to ui
	for (int32 i = 0; i < 30; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Slot index %i"), i);
		// Create new slot widget
		UInventorySlotWidget* NewInventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		if (i < 6) // Slot is less than 6
		{
			// Make it a toolbar slot
			NewInventorySlot->Setup(this, true);
			ToolbarWrapBox->AddChild(NewInventorySlot);
		}
		else
		{
			// Make it a standard inventory slot
			NewInventorySlot->Setup(this, false);
			InventoryWrapBox->AddChild(NewInventorySlot);
		}
		// Add this slot to slot array
		InventorySlots.Add(NewInventorySlot);
	}

	// Set default visibility
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryWrapBox->SetVisibility(ESlateVisibility::Hidden);
	ToolbarWrapBox->SetVisibility(ESlateVisibility::Visible);
}

bool UInventoryWidget::AddItem(FName ItemName, int32 Quantity, int32& AmountAdded, int32& Remaining)
{
	FItem* ItemData = InventoryComponent->GetItemData(ItemName);
	int32 QuantityToAdd = Quantity;
	AmountAdded = 0;
	if (ItemData == nullptr)
	{
		return false;
	}
	// Loop through all slots
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		// If the current slot's item is the same as the item we are adding
		if (InventorySlot->GetCurrentItemName() == ItemName && !InventorySlot->IsFull()) 
		{
			if ((InventorySlot->GetCurrentItemQuantity() + QuantityToAdd) > ItemData->StackSize)
			{
				// Remove the amount we are going to add to the slot
				
				// TODO something seems fishy here
				QuantityToAdd -= ItemData->StackSize - InventorySlot->GetCurrentItemQuantity();
				AmountAdded += ItemData->StackSize - InventorySlot->GetCurrentItemQuantity();
				UE_LOG(LogTemp, Warning, TEXT("Hey amount added sum: %i"), ItemData->StackSize - InventorySlot->GetCurrentItemQuantity());
				// Set the slot quantity to the stack size
				InventorySlot->SetItem(InventorySlot->GetCurrentItemName(), ItemData->StackSize);
				
				if (QuantityToAdd == 0)
				{
					break;
				}
			}
			else
			{
				AmountAdded += QuantityToAdd;
				InventorySlot->SetItem(InventorySlot->GetCurrentItemName(), InventorySlot->GetCurrentItemQuantity() + QuantityToAdd);
				if (QuantityToAdd == 0)
				{
					break;
				}
			}

		}
		// If the current slot has no item
		if (InventorySlot->GetCurrentItemQuantity() == 0)
		{
			if (QuantityToAdd > ItemData->StackSize)
			{
				// Remove the amount we are going to add to the slot
				QuantityToAdd -= ItemData->StackSize;
				AmountAdded += ItemData->StackSize;
				// Set the slot quantity to the stack size
				InventorySlot->SetItem(ItemName, ItemData->StackSize);
				if (QuantityToAdd == 0)
				{
					break;
				}
			}
			else
			{
				AmountAdded += QuantityToAdd;
				InventorySlot->SetItem(ItemName, QuantityToAdd);
				break;
			}
		}
	}
	Remaining = QuantityToAdd;
	return QuantityToAdd == 0 || AmountAdded > 0;
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

void UInventoryWidget::StartDragging(FName ItemName, int32 Quantity)
{
	FItem* ItemData = InventoryComponent->GetItemData(ItemName);
	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Item ID: %s"), *ItemName.ToString());
		return;
	}
	SelectedItem.Name = ItemName;
	SelectedItem.Quantity = Quantity;
	SelectedItemWidget->SetItem(ItemData->Thumbnail, Quantity);
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

FSelectedItem* UInventoryWidget::GetSelectedItem()
{
	return &SelectedItem;
}