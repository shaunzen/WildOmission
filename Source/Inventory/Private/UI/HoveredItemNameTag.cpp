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
	PromptTextBlock = nullptr;
	AdditionalInformationPanel = nullptr;
	DescriptionTextBlock = nullptr;
	StatsPanel = nullptr;

	PromptTimer = 0.0f;

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

	PromptTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	ShowAdditionalDetails(false);
}

void UHoveredItemNameTag::ShowAdditionalDetails(bool Show)
{
	AdditionalInformationPanel->SetVisibility(Show ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	PromptTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}

void UHoveredItemNameTag::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
	PromptTimer = 0.0f;
}

void UHoveredItemNameTag::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// todo count and then show the prompt
	PromptTimer += InDeltaTime;
	if (PromptTimer > 3.0f && AdditionalInformationPanel->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		PromptTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

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
	FItemData* ItemData = UInventoryComponent::GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		return TArray<FItemStat>();
	}

	return ItemData->Stats;
}
