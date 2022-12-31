// (c) 2023 Telephone Studios, all rights reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/Border.h"
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

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}