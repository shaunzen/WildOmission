// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WildOmission/Core/Structs/Notification.h"
#include "NotificationWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class WILDOMISSION_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UNotificationWidget(const FObjectInitializer& ObjectInitializer);
	void Setup(const FNotification& InNotification);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void HandleAddition();
	void HandleRemoval();

	bool IsInfinite() const;
	FNotification GetNotification() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UBorder* Icon;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	FNotification Notification;


	float SlideDuration;
	float SlideInStartTime;
	float SlideOutStartTime;
	bool ShouldSlideIn;
	bool ShouldSlideOut;

	void HandleSlideIn();
	void HandleSlideOut();

};
