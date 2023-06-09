// Copyright Telephone Studios. All Rights Reserved.


#include "NotificationWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UNotificationWidget::Setup(const FNotification& InNotification)
{
	Notification = InNotification;

	Icon->SetBrushFromMaterial(Notification.Icon);
	TextBlock->SetText(FText::FromString(Notification.Message));
}

void UNotificationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// TODO lifetime stuff
}
