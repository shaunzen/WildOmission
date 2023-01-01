// (c) 2023 Telephone Studios, all rights reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.h"
#include "../ActorComponents/InventoryComponent.h"

void UInventorySlotWidget::Setup(UInventoryWidget* InOwner, bool bInToolbarSlot)
{
	if (InOwner == nullptr || SlotButton == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	CurrentItemName = FName("");
	CurrentItemQuantity = 0;
	bToolbarSlot = bInToolbarSlot;
	SetItem(CurrentItemName, CurrentItemQuantity);
	
	SlotButton->OnPressed.AddDynamic(this, &UInventorySlotWidget::OnPressed);
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(FName ItemName, int32 ItemQuantity)
{
	CurrentItemName = ItemName;
	CurrentItemQuantity = ItemQuantity;

	FString QuantityString;
	if (CurrentItemQuantity > 1)
	{
		QuantityString = FString::Printf(TEXT("%i"), CurrentItemQuantity);
	}
	else
	{
		QuantityString = FString("");
	}

	if (CurrentItemQuantity != 0)
	{
		FItem* SlotItemData = Owner->GetInventoryComponent()->GetItemData(CurrentItemName);
		if (SlotItemData == nullptr)
		{
			return;
		}
		ItemIconBorder->SetBrushFromTexture(SlotItemData->Thumbnail);
		ItemIconBorder->SetBrushColor(FLinearColor::White);
	}
	else
	{
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
	QuantityText->SetText(FText::FromString(QuantityString));
}

void UInventorySlotWidget::OnPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Slot pressed."));
}

FName UInventorySlotWidget::GetCurrentItemName()
{
	return CurrentItemName;
}

int32 UInventorySlotWidget::GetCurrentItemQuantity()
{
	return CurrentItemQuantity;
}

/*
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
*/