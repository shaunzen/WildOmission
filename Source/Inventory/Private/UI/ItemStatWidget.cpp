// Copyright Telephone Studios. All Rights Reserved.


#include "UI/ItemStatWidget.h"
#include "Structs/ItemStat.h"
#include "Components/TextBlock.h"

UItemStatWidget::UItemStatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	StatNameTextBlock = nullptr;
	StatValueTextBlock = nullptr;
}

void UItemStatWidget::Setup(const FItemStat& InStat)
{
	if (StatNameTextBlock == nullptr || StatValueTextBlock == nullptr)
	{
		return;
	}
	// TODO look into this
	// TODO also advanced checking for ratio type things
	//InStat.Name.NameToDisplayString()
	FString StatNameString = InStat.Name.ToString();
	FString StatValueString = FString::Printf(TEXT("%i"), InStat.Value);
	StatNameTextBlock->SetText(FText::FromString(StatNameString));
	StatValueTextBlock->SetText(FText::FromString(StatValueString));
}
