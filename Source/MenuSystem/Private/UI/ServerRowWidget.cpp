// Copyright Telephone Studios. All Rights Reserved.


#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "ServerBrowserWidget.h"
#include "Color/UIColors.h"

void UServerRowWidget::Setup(UServerBrowserWidget* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnClicked);
}

void UServerRowWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UServerRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Hovering = true;
}

void UServerRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Hovering = false;
}

void UServerRowWidget::OnClicked()
{
	Parent->SelectServerIndex(Index);
}