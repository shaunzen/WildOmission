// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/CanvasPanel.h"
#include "../ActorComponents/InventoryComponent.h"

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

void UInventoryWidget::AddItem(FName ItemName, int32 Quantity)
{
	bool bSlotFound = false;
	// Loop through all slots
	for (UInventorySlotWidget* InventorySlot : InventorySlots)
	{
		// If the current slot's item is the same as the item we are adding
		if (InventorySlot->GetCurrentItemName() == ItemName)
		{
			InventorySlot->SetItem(InventorySlot->GetCurrentItemName(), InventorySlot->GetCurrentItemQuantity() + Quantity);
			bSlotFound = true;
			break;
		}
		// If the current slot has no item
		if (InventorySlot->GetCurrentItemQuantity() == 0)
		{
			InventorySlot->SetItem(ItemName, Quantity);
			bSlotFound = true;
			break;
		}
	}
	if (bSlotFound == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find a good slot to put the new item in."));
	}
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