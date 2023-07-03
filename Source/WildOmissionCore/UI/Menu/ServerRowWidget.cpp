// Copyright Telephone Studios. All Rights Reserved.


#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "ServerBrowserWidget.h"
#include "UIHelpers.h"

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