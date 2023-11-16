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

	if (Selected)
	{
		FUIColor* Blue = UUIColors::GetBaseColor(TEXT("Blue"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UUIColors::GetHighlightedColor(Blue) * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			RowBorder->SetBrushColor(Blue->Default * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
	else
	{
		FUIColor* DarkGray = UUIColors::GetBaseColor(TEXT("DarkGray"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UUIColors::GetHighlightedColor(DarkGray) * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			RowBorder->SetBrushColor(DarkGray->Default * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
}

void UWorldRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Hovering = true;
}

void UWorldRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Hovering = false;
}

void UWorldRowWidget::OnClicked()
{
	Parent->SetSelectedWorld(WorldName);
}