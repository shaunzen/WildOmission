// (c) 2023 Telephone Studios, all rights reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "../ActorComponents/InventoryComponent.h"

void UInventoryWidget::Setup(UInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	// Check for if inventory and slot are valid pointers
	if (InventoryComponent == nullptr || InventorySlotWidgetClass == nullptr)
	{
		return;
	}

	// Add Inventory slots to ui
	for (int32 i = 0; i < 30; ++i)
	{
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
}

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}