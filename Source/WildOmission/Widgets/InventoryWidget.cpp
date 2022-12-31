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
		UE_LOG(LogTemp, Warning, TEXT("Adding slot to inventory."));
		UInventorySlotWidget* NewInventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		NewInventorySlot->Setup(this);
		InventorySlots.Add(NewInventorySlot);
		InventoryContentsWrapBox->AddChild(NewInventorySlot);
	}
}

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}