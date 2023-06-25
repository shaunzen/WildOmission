// Copyright Telephone Studios. All Rights Reserved.


#include "UI/PlayerInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/PlayerInventoryComponent.h"

UPlayerInventoryWidget::UPlayerInventoryWidget(const FObjectInitializer& ObjectInitializer) : UInventoryWidget(ObjectInitializer)
{

}

void UPlayerInventoryWidget::Setup(UUserWidget* InParentHUD, UInventoryComponent* InInventoryComponent)
{
	Super::Setup(InParentHUD, InInventoryComponent);

	if (InInventoryComponent == nullptr)
	{
		return;
	}

	PlayerInventoryComponent = Cast<UPlayerInventoryComponent>(InInventoryComponent);
	if (PlayerInventoryComponent == nullptr)
	{
		return;
	}

	CreateToolbarSlots();
	CreateInventorySlots();

	ToolbarGridPanel->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerInventoryWidget::Refresh()
{
	Super::Refresh();

	if (PlayerInventoryComponent == nullptr || PlayerInventoryComponent->GetToolbarSelectionIndex() == -1)
	{
		return;
	}

	Slots[PlayerInventoryComponent->GetToolbarSelectionIndex()]->SetSelected(true);
}

void UPlayerInventoryWidget::CreateInventorySlots()
{
	uint8 Column = 0;
	uint8 Row = 0;

	for (uint8 i = 0; i < 24; ++i)
	{
		uint8 SlotIndex = i + 6;
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		NewSlot->Setup(this, SlotIndex);

		InventoryGridPanel->AddChild(NewSlot);

		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewSlot->Slot);

		GridSlot->SetColumn(Column);
		GridSlot->SetRow(Row);

		Slots.Add(NewSlot);

		++Column;
		if (Column > 5)
		{
			Column = 0;
			++Row;
		}
	}
}

void UPlayerInventoryWidget::CreateToolbarSlots()
{
	for (uint8 i = 0; i < 6; ++i)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		NewSlot->Setup(this, i);

		ToolbarGridPanel->AddChild(NewSlot);

		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewSlot->Slot);

		GridSlot->SetColumn(i);
		GridSlot->SetRow(0);

		Slots.Add(NewSlot);
	}
}