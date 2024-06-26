// Copyright Telephone Studios. All Rights Reserved.


#include "UI/CheatItemButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/InventoryComponent.h"

UCheatItemButtonWidget::UCheatItemButtonWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ItemID = NAME_None;
	ItemIcon = nullptr;
	ItemDisplayName = nullptr;
}

void UCheatItemButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCheatItemButtonWidget::SetItemID(const FName& NewItemID)
{
	ItemID = NewItemID;

	FItemData* ItemData = UInventoryComponent::GetItemData(ItemID);
	if (ItemData == nullptr)
	{
		return;
	}

	ItemDisplayName->SetText(FText::FromString(ItemData->DisplayName));
	ItemIcon->SetBrushFromMaterial(ItemData->Thumbnail);
}

FName UCheatItemButtonWidget::GetItemID() const
{
	return ItemID;
}

void UCheatItemButtonWidget::GiveItem(bool Stack)
{
}
