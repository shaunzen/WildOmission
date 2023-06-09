// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationPanelWidget.generated.h"

class UPanelWidget;
class UNotificationWidget;

UCLASS()
class WILDOMISSION_API UNotificationPanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UNotificationPanelWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* NotificationContainer;

	UPROPERTY()
	TSubclassOf<UNotificationWidget> NotificationWidgetBlueprint;

	UFUNCTION()
	void CreateItemNotification(const FInventoryItemUpdate& ItemUpdate);

	void AddNotification(const FNotification& Notification);
};
