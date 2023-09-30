// Copyright Telephone Studios. All Rights Reserved.


#include "UI/ItemStatWidget.h"
#include "Structs/ItemStat.h"
#include "Components/TextBlock.h"

UItemStatWidget::UItemStatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	StatNameTextBlock = nullptr;
	StatValueTextBlock = nullptr;
}

void UItemStatWidget::Setup(const FItemStat& DefaultStat, const FItemStat& CurrentStat)
{
	if (StatNameTextBlock == nullptr || StatValueTextBlock == nullptr)
	{
		return;
	}
	
	FString StatNameString = FName::NameToDisplayString(DefaultStat.Name.ToString(), false);
	FString StatValueString = FString::Printf(TEXT("%i"), DefaultStat.Value);

	const bool IsRatio = CurrentStat.Name == TEXT("Durability");
	if (IsRatio)
	{
		StatValueString = FString::Printf(TEXT("%i/%i"), CurrentStat.Value, DefaultStat.Value);
	}

	StatNameTextBlock->SetText(FText::FromString(StatNameString));
	StatValueTextBlock->SetText(FText::FromString(StatValueString));
}
