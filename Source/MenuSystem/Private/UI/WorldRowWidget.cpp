// Copyright Telephone Studios. All Rights Reserved.


#include "WorldRowWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "WorldSelectionWidget.h"
#include "Color/UIColors.h"

void UWorldRowWidget::Setup(UWorldSelectionWidget* InParent, const FString& InWorldName)
{
	Parent = InParent;
	WorldName = InWorldName;
	
	RowButton->OnClicked.AddDynamic(this, &UWorldRowWidget::OnClicked);
}

FString UWorldRowWidget::GetWorldName()
{
	return WorldName;
}

void UWorldRowWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FUIColor* Blue = UUIColors::GetBaseColor(TEXT("Blue"));
	FUIColor* LightGray = UUIColors::GetBaseColor(TEXT("LightGray"));
	if (Blue == nullptr || LightGray == nullptr)
	{
		return;
	}

	FLinearColor ButtonColor = Selected ? Blue->Default : LightGray->Default;
	RowButton->SetBackgroundColor(ButtonColor * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
}

void UWorldRowWidget::OnClicked()
{
	Parent->SetSelectedWorld(WorldName);
}