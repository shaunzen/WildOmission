// (c) 2023 Telephone Studios, all rights reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "../ActorComponents/InventoryComponent.h"

void UInventorySlotWidget::Setup(FName InItemName, int32 InQuantity, UInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent == nullptr)
	{
		return;
	}
	
	ItemName = InItemName;
	Quantity = InQuantity;
	OwnerInventoryComponent = InInventoryComponent;
	
	//************************************
	// Amount Text
	FString QuantityString;
	
	if (Quantity > 1)
	{
		// Only Set the string to a number if there is more than one
		QuantityString = FString::Printf(TEXT("%i"), Quantity);
	}
	else
	{
		QuantityString = FString("");
	}
	QuantityText->SetText(FText::FromString(QuantityString));

	//************************************
	// Image Icon
	FItem* SlotItemData = OwnerInventoryComponent->GetItemData(ItemName);
	
	if (SlotItemData == nullptr)
	{
		return;
	}

	SlotImageBorder->SetBrushFromTexture(SlotItemData->Thumbnail);
	SlotImageBorder->SetBrushColor(FLinearColor::White);
}