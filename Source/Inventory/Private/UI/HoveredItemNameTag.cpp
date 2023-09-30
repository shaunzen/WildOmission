// Copyright Telephone Studios. All Rights Reserved.


#include "UI/HoveredItemNameTag.h"
#include "Components/InventoryComponent.h"
#include "Structs/ItemData.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/ItemStatWidget.h"
#include "UObject/ConstructorHelpers.h"

UHoveredItemNameTag::UHoveredItemNameTag(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	StatWidgetClass = nullptr;
	NameTextBlock = nullptr;
	AdditionalInformationPanel = nullptr;
	DescriptionTextBlock = nullptr;
	StatsPanel = nullptr;

	SetVisibility(ESlateVisibility::Hidden);

	ConstructorHelpers::FClassFinder<UItemStatWidget> StatWidgetBlueprint(TEXT("/Game/Inventory/UI/WBP_ItemStat"));
	if (StatWidgetBlueprint.Succeeded())
	{
		StatWidgetClass = StatWidgetBlueprint.Class;
	}
}

void UHoveredItemNameTag::NativeConstruct()
{
	Super::NativeConstruct();

	Hide();
}

void UHoveredItemNameTag::Show(const FInventoryItem& Item)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);

	FString NameString = FString::Printf(TEXT("%s (%i)"), *GetItemDisplayName(Item), Item.UniqueID);

	NameTextBlock->SetText(FText::FromString(NameString));
	DescriptionTextBlock->SetText(FText::FromString(GetItemDescription(Item)));

	if (StatWidgetClass == nullptr)
	{
		return;
	}

	TArray<FItemStat> DefaultItemStats = GetDefaultItemStats(Item);
	TArray<FItemStat> CurrentItemStats = Item.Stats;
	StatsPanel->ClearChildren();


	for (int32 i = 0; i < DefaultItemStats.Num(); i++)
	{
		if (StatWidgetClass == nullptr)
		{
			return;
		}
		UItemStatWidget* StatWidget = CreateWidget<UItemStatWidget>(this, StatWidgetClass);
		if (StatWidget == nullptr || !DefaultItemStats.IsValidIndex(i) || !CurrentItemStats.IsValidIndex(i))
		{
			continue;
		}

		StatWidget->Setup(DefaultItemStats[i], CurrentItemStats[i]);
		StatsPanel->AddChild(StatWidget);
	}
}

void UHoveredItemNameTag::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

// TODO make into some kind of statics class
FString UHoveredItemNameTag::GetItemDisplayName(const FInventoryItem& Item) const
{
	FItemData* ItemData = UInventoryComponent::GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		return TEXT("");
	}

	return ItemData->DisplayName;
}

FString UHoveredItemNameTag::GetItemDescription(const FInventoryItem& Item) const
{
	FItemData* ItemData = UInventoryComponent::GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		return TEXT("");
	}

	return ItemData->Description;
}

TArray<FItemStat> UHoveredItemNameTag::GetDefaultItemStats(const FInventoryItem& Item) const
{
	TArray<FItemStat> Stats;
	FItemData* ItemData = UInventoryComponent::GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		Stats;
	}

	return ItemData->Stats;
}
