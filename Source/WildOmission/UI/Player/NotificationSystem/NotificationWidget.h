// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class WILDOMISSION_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

private:
	UPROPERTY(Meta = (BindWidget))
	UBorder* NotificationIcon;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* NotificationTextBlock;

};
