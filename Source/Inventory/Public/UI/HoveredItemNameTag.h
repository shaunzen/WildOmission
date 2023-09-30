// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HoveredItemNameTag.generated.h"

struct FInventoryItem;
struct FItemStat;
class UTextBlock;
class UItemStatWidget;

UCLASS()
class INVENTORY_API UHoveredItemNameTag : public UUserWidget
{
	GENERATED_BODY()
public:
	UHoveredItemNameTag(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Show(const FInventoryItem& Item);
	void ShowAdditionalDetails(bool Show);
	void Hide();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemStatWidget> StatWidgetClass;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* PromptTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* AdditionalInformationPanel;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DescriptionTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* StatsPanel;

	float PromptTimer;

	FString GetItemDisplayName(const FInventoryItem& Item) const;
	FString GetItemDescription(const FInventoryItem& Item) const;
	TArray<FItemStat> GetDefaultItemStats(const FInventoryItem& Item) const;
};
