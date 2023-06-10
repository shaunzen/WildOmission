// Copyright Telephone Studios. All Rights Reserved.


#include "NotificationPanelWidget.h"
#include "NotificationWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "WildOmission/Core/Structs/Notification.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"
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

void UNotificationPanelWidget::CreateItemNotification(const FInventoryItemUpdate& ItemUpdate)
{
	FNotification Notification;
	Notification.Time = GetWorld()->GetRealTimeSeconds();
	Notification.Duration = 5.0f;

	FItemData* ItemData = UWildOmissionStatics::GetItemData(ItemUpdate.Item.Name);
	if (ItemData == nullptr)
	{
		return;
	}

	Notification.Icon = ItemData->Thumbnail;
	if (ItemUpdate.Addition == true)
	{
		Notification.Message = FString::Printf(TEXT("+ %i %s"), ItemUpdate.Item.Quantity, *ItemData->DisplayName);
	}
	else
	{
		Notification.Message = FString::Printf(TEXT("- %i %s"), ItemUpdate.Item.Quantity, *ItemData->DisplayName);
	}

	AddNotification(Notification);
}

void UNotificationPanelWidget::AddNotification(const FNotification& Notification)
{
	UNotificationWidget* NotificationWidget = CreateWidget<UNotificationWidget>(this, NotificationWidgetBlueprint);
	if (NotificationWidget == nullptr)
	{
		return;
	}

	NotificationWidget->Setup(Notification);

	for (UWidget* ChildNotification : NotificationContainer->GetAllChildren())
	{
		UGridSlot* ChildNotificationSlot = Cast<UGridSlot>(ChildNotification->Slot);
		ChildNotificationSlot->SetRow(ChildNotificationSlot->GetRow() + 1);
	}

	NotificationContainer->AddChild(NotificationWidget);
}