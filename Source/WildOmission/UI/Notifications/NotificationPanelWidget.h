// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationPanelWidget.generated.h"

class UNotificationWidget;

UCLASS()
class WILDOMISSION_API UNotificationPanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UNotificationPanelWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	TSubclassOf<UNotificationWidget> NotificationWidgetBlueprint;

	UFUNCTION()
		void CreateItemNotification(const FInventoryItemUpdate& ItemUpdate);


};
