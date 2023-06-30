// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationPanelWidget.generated.h"

class UGridPanel;
class UNotificationWidget;
struct FNotification;

UCLASS()
class NOTIFICATIONS_API UNotificationPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UNotificationPanelWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UGridPanel* NotificationContainer;
	
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* StatusNotificationContainer;

	UPROPERTY()
	TSubclassOf<UNotificationWidget> NotificationWidgetBlueprint;

	UFUNCTION()
	void CreateItemNotification(const FInventoryItemUpdate& ItemUpdate);

	UFUNCTION()
	void AddThirstyNotification(const float& Time);
	UFUNCTION()
	void RemoveThirstyNotification(const float& Time);

	UFUNCTION()
	void AddStarvingNotification(const float& Time);
	UFUNCTION()
	void RemoveStarvingNotification(const float& Time);

	void AddNotification(const FNotification& Notification);
	void RemoveNotification(const FName& NotificationName);
};
