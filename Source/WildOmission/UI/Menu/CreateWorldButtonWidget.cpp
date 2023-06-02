// Copyright Telephone Studios. All Rights Reserved.


#include "CreateWorldButtonWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "WildOmission/Core/WildOmissionStatics.h"

UButton* UCreateWorldButtonWidget::GetButton() const
{
	return Button;
}

void UCreateWorldButtonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UCreateWorldButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Hovering = true;
}

void UCreateWorldButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Hovering = false;
}
