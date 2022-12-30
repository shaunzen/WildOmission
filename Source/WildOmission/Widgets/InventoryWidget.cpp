// (c) 2023 Telephone Studios, all rights reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "../ActorComponents/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Check for if inventory and slot are valid pointers
	if (InventoryComponent == nullptr || InventorySlotWidgetClass == nullptr)
	{
		return;
	}

	for (const TPair<FName, int32>& Pair : *InventoryComponent->GetContent())
	{
		UInventorySlotWidget* NewInventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		InventoryContentsWrapBox->AddChild(NewInventorySlot);
	}
}