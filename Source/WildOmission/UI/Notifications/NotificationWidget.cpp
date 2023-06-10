// Copyright Telephone Studios. All Rights Reserved.


#include "NotificationWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/WildOmissionStatics.h"

void UNotificationWidget::Setup(const FNotification& InNotification)
{
	Notification = InNotification;

	Icon->SetBrushFromMaterial(Notification.Icon);
	TextBlock->SetText(FText::FromString(Notification.Message));

	FWidgetTransform NewTransform;
	NewTransform.Translation.X = 1000.0f;
	SetRenderTransform(NewTransform);
}

void UNotificationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	FWidgetTransform NewTransform;
	float NewX = UWildOmissionStatics::GetSwoopLerp(500.0f, 0.0f, 10.0f, GetWorld()->GetRealTimeSeconds(), Notification.Time, Notification.Time + Notification.Duration);
	NewTransform.Translation.X = NewX;
	SetRenderTransform(NewTransform);

	if (!IsInfinite() && GetWorld()->GetRealTimeSeconds() > Notification.Time + Notification.Duration)
	{
		RemoveFromParent();
	}
}

bool UNotificationWidget::IsInfinite() const
{
	return Notification.Duration == 0.0f;
}
