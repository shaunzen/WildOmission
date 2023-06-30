// Copyright Telephone Studios. All Rights Reserved.


#include "WorldRowWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "WorldSelectionWidget.h"
#include "UIHelpers.h"

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
		FUIColor* Blue = UUIHelpers::GetUIColor(FName("Blue"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UUIHelpers::GetHighlightedColor(Blue));
		}
		else
		{
			RowBorder->SetBrushColor(Blue->Default);
		}
	}
	else
	{
		FUIColor* DarkGray = UUIHelpers::GetUIColor(FName("DarkGray"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UUIHelpers::GetHighlightedColor(DarkGray));
		}
		else
		{
			RowBorder->SetBrushColor(DarkGray->Default);
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