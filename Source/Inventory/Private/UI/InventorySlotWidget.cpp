// Copyright Telephone Studios. All Rights Reserved.


#include "UI/InventorySlotWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/HoveredItemNameTag.h"
#include "UI/PlayerInventoryWidget.h"
#include "UI/InventoryMenuWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Color/UIColors.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SlotBorder = nullptr;
	ItemIconBorder = nullptr;
	QuantityText = nullptr;
	DurabilityBar = nullptr;
	Owner = nullptr;
	Index = -1;
	CurrentItemName = FName();
	CurrentItemQuantity = 0;
	Selected = false;
	Hovering = false;
}

void UInventorySlotWidget::Setup(UInventoryWidget* InOwner, const int32& InIndex)
{
	if (InOwner == nullptr || SlotBorder == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	Index = InIndex;
	
	ClearItem();
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(const FInventoryItem& Item)
{
	FString QuantityString;
	if (Item.Quantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), Item.Quantity);
	}
	else
	{
		QuantityString = FString("");
	}

	if (Item.Quantity != 0)
	{
		CurrentItemName = Item.Name;
		CurrentItemQuantity = Item.Quantity;

		FItemData* SlotItemData = UInventoryComponent::GetItemData(Item.Name);
		if (SlotItemData == nullptr)
		{
			return;
		}

		// Set the item icon border to the item thumbnail
		ItemIconBorder->SetBrushFromMaterial(SlotItemData->Thumbnail);
		// Set the item icon color opaque white
		ItemIconBorder->SetBrushColor(FLinearColor::White);

		if (SlotItemData->GetStat(TEXT("Durability")) != INDEX_NONE)
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Visible);
			float Percent;
			Percent = (float)Item.GetStat(TEXT("Durability")) / (float)SlotItemData->GetStat(TEXT("Durability"));
			DurabilityBar->SetPercent(Percent);
			DurabilityBar->SetFillColorAndOpacity(FMath::Lerp(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), Percent));
		}
		else
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
		}

		if (SlotItemData->GetStat(TEXT("MaxAmmo")) != INDEX_NONE)
		{
			QuantityString = FString::Printf(TEXT("%i/%i"), Item.GetStat(TEXT("CurrentAmmo")), Item.GetStat(TEXT("MaxAmmo")));
		}
	}
	else
	{
		CurrentItemName = NAME_None;
		CurrentItemQuantity = 0;
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
	}
	QuantityText->SetText(FText::FromString(QuantityString));
}

void UInventorySlotWidget::ClearItem()
{
	FInventoryItem BlankItem;

	this->SetItem(BlankItem);
}

void UInventorySlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsSelected())
	{
		FUIColor* Blue = UUIColors::GetBaseColor(FName("Blue"));
		if (Blue == nullptr)
		{
			return;
		}

		if (Hovering)
		{
			SlotBorder->SetBrushColor(UUIColors::GetHighlightedColor(Blue));
		}
		else
		{
			SlotBorder->SetBrushColor(Blue->Default);
		}
	}
	else
	{
		FUIColor* LightGray = UUIColors::GetBaseColor(FName("LightGray"));
		if (LightGray == nullptr)
		{
			return;
		}

		if (Hovering)
		{
			SlotBorder->SetBrushColor(UUIColors::GetHighlightedColor(LightGray));
		}
		else
		{
			SlotBorder->SetBrushColor(LightGray->Default);
		}
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr || !Owner->IsOpen())
	{
		return FReply::Handled();
	}

	UInventoryManipulatorComponent* OwnerInventoryManipulatorComponent = PlayerPawn->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulatorComponent == nullptr)
	{
		return FReply::Handled();
	}
	
	bool QuickMove = InMouseEvent.GetModifierKeys().IsLeftShiftDown() || InMouseEvent.GetModifierKeys().IsRightShiftDown();

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Owner->GetInventoryComponent()->SlotInteraction(this->Index, OwnerInventoryManipulatorComponent, QuickMove, true);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		Owner->GetInventoryComponent()->SlotInteraction(this->Index, OwnerInventoryManipulatorComponent, QuickMove, false);
	}

	HideHoveredItemNameTag();

	return FReply::Handled();
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	ShowHoveredItemNameTag();
	Hovering = true;
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	HideHoveredItemNameTag();
	Hovering = false;
}

int32 UInventorySlotWidget::GetIndex() const
{
	return Index;
}

void UInventorySlotWidget::SetSelected(bool InSelected)
{
	Selected = InSelected;
}

bool UInventorySlotWidget::IsSelected() const
{
	return Selected;
}

void UInventorySlotWidget::ShowHoveredItemNameTag()
{
	if (CurrentItemQuantity == 0)
	{
		return;
	}

	UInventoryComponent* OwnerInventory = Owner->GetInventoryComponent();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	UInventoryMenuWidget* InventoryParentMenu = Owner->GetParentMenu();
	if (InventoryParentMenu == nullptr || InventoryParentMenu->SelectedItemVisible() || InventoryParentMenu->GetHoveredItemNameTagWidget() == nullptr)
	{
		return;
	}
	
	FInventorySlot* ThisSlot = OwnerInventory->GetSlot(Index);
	if (ThisSlot == nullptr)
	{
		return;
	}

	InventoryParentMenu->GetHoveredItemNameTagWidget()->Show(ThisSlot->Item);
}

void UInventorySlotWidget::HideHoveredItemNameTag()
{
	UInventoryMenuWidget* InventoryParentMenu = Owner->GetParentMenu();
	if (InventoryParentMenu == nullptr || InventoryParentMenu->GetHoveredItemNameTagWidget() == nullptr)
	{
		return;
	}

	InventoryParentMenu->GetHoveredItemNameTagWidget()->Hide();
}