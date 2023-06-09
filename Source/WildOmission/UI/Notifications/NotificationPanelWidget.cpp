// Copyright Telephone Studios. All Rights Reserved.


#include "NotificationPanelWidget.h"
#include "NotificationWidget.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "UObject/ConstructorHelpers.h"

UNotificationPanelWidget::UNotificationPanelWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UNotificationWidget> NotificationWidgetBlueprintClass(TEXT("/Game/WildOmission/UI/Notifications/WBP_Notification"));
	if (NotificationWidgetBlueprintClass.Succeeded())
	{
		NotificationWidgetBlueprint = NotificationWidgetBlueprintClass.Class;
	}
}

void UNotificationPanelWidget::NativeConstruct()
{
	// We want to add notif if we add an item to our inventory, or remove
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}

	OwnerInventoryComponent->OnItemUpdate.AddDynamic(this, &UNotificationPanelWidget::CreateItemNotification);
}

void UNotificationPanelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

void UNotificationPanelWidget::CreateItemNotification(const FInventoryItemUpdate& ItemUpdate)
{
	if (ItemUpdate.Addition == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("%i %s was added to inventory."), ItemUpdate.Item.Quantity, *ItemUpdate.Item.Name.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%i %s was removed from inventory."), ItemUpdate.Item.Quantity, *ItemUpdate.Item.Name.ToString());
	}
}