// Copyright Telephone Studios. All Rights Reserved.


#include "UI/NotificationPanelWidget.h"
#include "NotificationWidget.h"
#include "Components/PanelWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Structs/Notification.h"
#include "Components/InventoryComponent.h"
#include "Components/VitalsComponent.h"
#include "UObject/ConstructorHelpers.h"

static UMaterialInterface* HealthIcon = nullptr;
static UMaterialInterface* ThirstIcon = nullptr;
static UMaterialInterface* HungerIcon = nullptr;

UNotificationPanelWidget::UNotificationPanelWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UNotificationWidget> NotificationWidgetBlueprintClass(TEXT("/Game/WildOmission/UI/Notifications/WBP_Notification"));
	if (NotificationWidgetBlueprintClass.Succeeded())
	{
		NotificationWidgetBlueprint = NotificationWidgetBlueprintClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HealthIconObject(TEXT("/Game/WildOmission/UI/Icons/Vitals/M_Health_Icon_Inst"));
	if (HealthIconObject.Succeeded())
	{
		HealthIcon = HealthIconObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ThirstIconObject(TEXT("/Game/WildOmission/UI/Icons/Vitals/M_Thirst_Icon_Inst"));
	if (ThirstIconObject.Succeeded())
	{
		ThirstIcon = ThirstIconObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HungerIconObject(TEXT("/Game/WildOmission/UI/Icons/Vitals/M_Hunger_Icon_Inst"));
	if (HungerIconObject.Succeeded())
	{
		HungerIcon = HungerIconObject.Object;
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

	UVitalsComponent* OwnerVitalsComponent = GetOwningPlayerPawn()->FindComponentByClass<UVitalsComponent>();
	if (OwnerVitalsComponent == nullptr)
	{
		return;
	}

	OwnerVitalsComponent->OnBeginThirst.AddDynamic(this, &UNotificationPanelWidget::AddThirstyNotification);
	OwnerVitalsComponent->OnEndThirst.AddDynamic(this, &UNotificationPanelWidget::RemoveThirstyNotification);
	OwnerVitalsComponent->OnBeginStarving.AddDynamic(this, &UNotificationPanelWidget::AddStarvingNotification);
	OwnerVitalsComponent->OnEndStarving.AddDynamic(this, &UNotificationPanelWidget::RemoveStarvingNotification);
}

void UNotificationPanelWidget::CreateItemNotification(const FInventoryItemUpdate& ItemUpdate)
{
	FNotification Notification;
	Notification.Negative = !ItemUpdate.Addition;
	Notification.Time = GetWorld()->GetRealTimeSeconds();
	Notification.Duration = 5.0f;

	FItemData* ItemData = UInventoryComponent::GetItemData(ItemUpdate.Item.Name);
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

void UNotificationPanelWidget::AddThirstyNotification(const float& Time)
{
	FNotification ThirstyNotification;
	ThirstyNotification.Negative = true;
	ThirstyNotification.Time = Time;
	ThirstyNotification.Duration = 0.0f;
	ThirstyNotification.Identifier = FName("Thirsty");
	ThirstyNotification.Message = FString("Thirsty");
	ThirstyNotification.Icon = ThirstIcon;

	AddNotification(ThirstyNotification);
}

void UNotificationPanelWidget::RemoveThirstyNotification(const float& Time)
{
	RemoveNotification(FName("Thirsty"));
}

void UNotificationPanelWidget::AddStarvingNotification(const float& Time)
{
	FNotification StarvingNotification;
	StarvingNotification.Negative = true;
	StarvingNotification.Time = Time;
	StarvingNotification.Duration = 0.0f;
	StarvingNotification.Identifier = FName("Starving");
	StarvingNotification.Message = FString("Starving");
	StarvingNotification.Icon = HungerIcon;

	AddNotification(StarvingNotification);
}

void UNotificationPanelWidget::RemoveStarvingNotification(const float& Time)
{
	RemoveNotification(FName("Starving"));
}

void UNotificationPanelWidget::AddNotification(const FNotification& Notification)
{
	UNotificationWidget* NotificationWidget = CreateWidget<UNotificationWidget>(this, NotificationWidgetBlueprint);
	if (NotificationWidget == nullptr)
	{
		return;
	}

	NotificationWidget->Setup(Notification);

	if (Notification.Duration == 0.0f)
	{
		StatusNotificationContainer->AddChild(NotificationWidget);
		return;
	}

	for (UWidget* ChildNotification : NotificationContainer->GetAllChildren())
	{
		UGridSlot* ChildNotificationSlot = Cast<UGridSlot>(ChildNotification->Slot);
		ChildNotificationSlot->SetRow(ChildNotificationSlot->GetRow() + 1);
	}

	NotificationContainer->AddChild(NotificationWidget);
}

void UNotificationPanelWidget::RemoveNotification(const FName& NotificationIdentifier)
{
	for (UWidget* ChildWidget : StatusNotificationContainer->GetAllChildren())
	{
		UNotificationWidget* ChildNotificationWidget = Cast<UNotificationWidget>(ChildWidget);
		if (ChildNotificationWidget == nullptr || ChildNotificationWidget->GetNotification().Identifier != NotificationIdentifier)
		{
			continue;
		}

		ChildNotificationWidget->HandleRemoval();
		return;
	}

	for (UWidget* ChildWidget : NotificationContainer->GetAllChildren())
	{
		UNotificationWidget* ChildNotificationWidget = Cast<UNotificationWidget>(ChildWidget);
		if (ChildNotificationWidget == nullptr || ChildNotificationWidget->GetNotification().Identifier != NotificationIdentifier)
		{
			continue;
		}

		ChildNotificationWidget->HandleRemoval();
		return;
	}
}
