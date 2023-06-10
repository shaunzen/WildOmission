// Copyright Telephone Studios. All Rights Reserved.


#include "NotificationWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/WildOmissionStatics.h"

UNotificationWidget::UNotificationWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SlideDuration = 0.1f;
	SlideInStartTime = 0.0f;
	SlideOutStartTime = 0.0f;
	ShouldSlideIn = false;
	ShouldSlideOut = false;
}

void UNotificationWidget::Setup(const FNotification& InNotification)
{
	Notification = InNotification;

	Icon->SetBrushFromMaterial(Notification.Icon);
	TextBlock->SetText(FText::FromString(Notification.Message));

	HandleAddition();
}

void UNotificationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (ShouldSlideIn)
	{
		HandleSlideIn();
	}
	else if (ShouldSlideOut)
	{
		HandleSlideOut();
	}
	else if (!IsInfinite() && GetWorld()->GetRealTimeSeconds() > Notification.Time + Notification.Duration)
	{
		HandleRemoval();
	}
}

void UNotificationWidget::HandleAddition()
{
	SlideInStartTime = GetWorld()->GetRealTimeSeconds();
	ShouldSlideIn = true;
}

void UNotificationWidget::HandleRemoval()
{
	SlideOutStartTime = GetWorld()->GetRealTimeSeconds();
	ShouldSlideOut = true;
}

bool UNotificationWidget::IsInfinite() const
{
	return Notification.Duration == 0.0f;
}

FNotification UNotificationWidget::GetNotification() const
{
	return Notification;
}

void UNotificationWidget::HandleSlideIn()
{
	FWidgetTransform NewTransform;
	float ZeroedTime = GetWorld()->GetRealTimeSeconds() - SlideInStartTime;
	float XValue = FMath::Lerp(500.0f, 0.0f, ZeroedTime / SlideDuration);

	if (XValue <= 0.0f)
	{
		XValue = 0.0f;
		ShouldSlideIn = false;
	}

	NewTransform.Translation.X = XValue;

	SetRenderTransform(NewTransform);
}

void UNotificationWidget::HandleSlideOut()
{
	FWidgetTransform NewTransform;
	float ZeroedTime = GetWorld()->GetRealTimeSeconds() - SlideOutStartTime;
	float XValue = FMath::Lerp(0.0f, 500.0f, ZeroedTime / SlideDuration);

	if (XValue >= 500.0f)
	{
		RemoveFromParent();
	}

	NewTransform.Translation.X = XValue;

	SetRenderTransform(NewTransform);
}
