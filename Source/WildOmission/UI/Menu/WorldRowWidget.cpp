// Copyright Telephone Studios. All Rights Reserved.


#include "WorldRowWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "WorldSelectionWidget.h"
#include "WildOmission/Core/WildOmissionStatics.h"

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
	if (Selected)
	{
		FUIColor* Blue = UWildOmissionStatics::GetUIColor(FName("Blue"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UWildOmissionStatics::GetHighlightedColor(Blue));
		}
		else
		{
			RowBorder->SetBrushColor(Blue->Default);
		}
	}
	else
	{
		FUIColor* DarkGray = UWildOmissionStatics::GetUIColor(FName("DarkGray"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UWildOmissionStatics::GetHighlightedColor(DarkGray));
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