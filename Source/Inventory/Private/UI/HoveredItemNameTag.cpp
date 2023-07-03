// Copyright Telephone Studios. All Rights Reserved.


#include "UI/HoveredItemNameTag.h"
#include "Components/TextBlock.h"

UHoveredItemNameTag::UHoveredItemNameTag(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UHoveredItemNameTag::NativeConstruct()
{
	Super::NativeConstruct();

	Hide();
}

void UHoveredItemNameTag::Show(const FString& ItemName)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);

	ItemNameTextBlock->SetText(FText::FromString(ItemName));
}

void UHoveredItemNameTag::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}