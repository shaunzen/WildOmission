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

	FItemData* ItemData = UInventoryComponent::GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		return;
	}
	
	NameTextBlock->SetText(FText::FromString(ItemData->DisplayName));
	DescriptionTextBlock->SetText(FText::FromString(ItemData->Description));

	if (StatWidgetClass == nullptr)
	{
		return;
	}

	TArray<FItemStat> CurrentItemStats = Item.Stats;
	TArray<FItemStat> DefaultItemStats = ItemData->Stats;

	StatsPanel->ClearChildren();
	for (int32 i = 0; i < DefaultItemStats.Num(); i++)
	{
		if (StatWidgetClass == nullptr)
		{
			return;
		}
		UItemStatWidget* StatWidget = CreateWidget<UItemStatWidget>(this, StatWidgetClass);
		if (StatWidget == nullptr || !CurrentItemStats.IsValidIndex(i) || !DefaultItemStats.IsValidIndex(i))
		{
			continue;
		}

		StatWidget->Setup(CurrentItemStats[i], DefaultItemStats[i]);
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
	
	const float TimeToShowPrompt = 2.0f;

	PromptTimer += InDeltaTime;
	if (PromptTimer > TimeToShowPrompt && AdditionalInformationPanel->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		PromptTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}