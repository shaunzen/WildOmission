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
	void Setup(const FNotification& InNotification);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UBorder* Icon;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	FNotification Notification;

};
