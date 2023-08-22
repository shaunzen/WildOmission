// Copyright Telephone Studios. All Rights Reserved.


#include "CreateWorldButtonWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Color/UIColors.h"

UButton* UCreateWorldButtonWidget::GetButton() const
{
	return Button;
}

void UCreateWorldButtonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FUIColor* DarkGray = UUIColors::GetBaseColor(FName("DarkGray"));

	if (Hovering)
	{
		RowBorder->SetBrushColor(UUIColors::GetHighlightedColor(DarkGray));
	}
	else
	{
		RowBorder->SetBrushColor(DarkGray->Default);
	}
}

void UCreateWorldButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Hovering = true;
}

void UCreateWorldButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Hovering = false;
}
