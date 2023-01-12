// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/CanvasPanel.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"

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

	// Add Inventory slots to ui
	for (int32 i = 0; i < 30; ++i)
	{
		// Create new slot widget
		UInventorySlotWidget* NewInventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		if (i < 6) // Slot is less than 6
		{
			// Make it a toolbar slot
			NewInventorySlot->Setup(this, true); // Instead of passing in an explicit bool value you could just pass i < 6 and it would also return the correct result
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
	
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}
		if (InventorySlot->IsFull())
		{
			continue;
		}

		// Are we trying to add the same item?
		if (InventorySlot->GetCurrentItemName() == ItemName)
		{
			//int32 AmountAddedToSlot;
			if (InventorySlot->GetCurrentItemQuantity() + QuantityToAdd > ItemData->StackSize)
			{
				QuantityToAdd -= ItemData->StackSize - InventorySlot->GetCurrentItemQuantity();
				InventorySlot->SetItem(ItemName, ItemData->StackSize);
			}
			else
			{
				InventorySlot->SetItem(ItemName, QuantityToAdd + InventorySlot->GetCurrentItemQuantity());
				QuantityToAdd = 0;
			}
		}
		// Is this slot empty instead?
		else if (InventorySlot->GetCurrentItemQuantity() == 0)
		{
			if (QuantityToAdd > ItemData->StackSize)
			{
				QuantityToAdd -= ItemData->StackSize;
				InventorySlot->SetItem(ItemName, ItemData->StackSize);
			}
			else
			{
				InventorySlot->SetItem(ItemName, QuantityToAdd);
				QuantityToAdd = 0;
			}
		}
	}
	Remaining = QuantityToAdd;
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