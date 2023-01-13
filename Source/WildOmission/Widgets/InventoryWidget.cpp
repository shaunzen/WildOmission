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

bool UInventoryWidget::AddItemToPopulatedSlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
{
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}
		if (InventorySlot->IsFull() || InventorySlot->GetCurrentItemName() != ItemName)
		{
			continue;
		}
		
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
		if (InventorySlot->IsFull() || InventorySlot->GetCurrentItemQuantity() > 0)
		{
			continue;
		}

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

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}