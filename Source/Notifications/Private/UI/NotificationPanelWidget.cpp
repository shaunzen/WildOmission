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
static UMaterialInterface* BuildingIcon = nullptr;

UNotificationPanelWidget::UNotificationPanelWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	StatusNotificationContainer = nullptr;
	NotificationContainer = nullptr;
	
	static ConstructorHelpers::FClassFinder<UNotificationWidget> NotificationWidgetBlueprintClass(TEXT("/Game/Notifications/UI/WBP_Notification"));
	if (NotificationWidgetBlueprintClass.Succeeded())
	{
		NotificationWidgetBlueprint = NotificationWidgetBlueprintClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HealthIconObject(TEXT("/Game/WildOmissionCore/UI/Icons/Vitals/M_Health_Icon_Inst"));
	if (HealthIconObject.Succeeded())
	{
		HealthIcon = HealthIconObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ThirstIconObject(TEXT("/Game/WildOmissionCore/UI/Icons/Vitals/M_Thirst_Icon_Inst"));
	if (ThirstIconObject.Succeeded())
	{
		ThirstIcon = ThirstIconObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HungerIconObject(TEXT("/Game/WildOmissionCore/UI/Icons/Vitals/M_Hunger_Icon_Inst"));
	if (HungerIconObject.Succeeded())
	{
		HungerIcon = HungerIconObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BuildingIconObject(TEXT("/Game/WildOmissionCore/UI/Icons/Vitals/M_Building_Icon_Inst"));
	if (BuildingIconObject.Succeeded())
	{
		BuildingIcon = BuildingIconObject.Object;
	}
}

void UNotificationPanelWidget::NativeConstruct()
{
	APawn* OwnerPawn = GetOwningPlayerPawn();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	// Bind inventory updates to create notifications
	if (UInventoryComponent* OwnerInventoryComponent = OwnerPawn->FindComponentByClass<UInventoryComponent>())
	{
		OwnerInventoryComponent->OnItemUpdate.AddDynamic(this, &UNotificationPanelWidget::CreateItemNotification);
	}
	
	// Bind vitals updates to create nofifications
	if (UVitalsComponent* OwnerVitalsComponent = OwnerPawn->FindComponentByClass<UVitalsComponent>())
	{
		OwnerVitalsComponent->OnBeginThirst.AddDynamic(this, &UNotificationPanelWidget::AddThirstyNotification);
		OwnerVitalsComponent->OnEndThirst.AddDynamic(this, &UNotificationPanelWidget::RemoveThirstyNotification);
		OwnerVitalsComponent->OnBeginStarving.AddDynamic(this, &UNotificationPanelWidget::AddStarvingNotification);
		OwnerVitalsComponent->OnEndStarving.AddDynamic(this, &UNotificationPanelWidget::RemoveStarvingNotification);
	}

	// Bind builder updates to create notifications
	// TODO!
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
	ThirstyNotification.Identifier = TEXT("Thirsty");
	ThirstyNotification.Message = TEXT("Thirsty");
	ThirstyNotification.Icon = ThirstIcon;

	AddNotification(ThirstyNotification);
}

void UNotificationPanelWidget::RemoveThirstyNotification(const float& Time)
{
	RemoveNotification(TEXT("Thirsty"));
}

void UNotificationPanelWidget::AddStarvingNotification(const float& Time)
{
	FNotification StarvingNotification;
	StarvingNotification.Negative = true;
	StarvingNotification.Time = Time;
	StarvingNotification.Duration = 0.0f;
	StarvingNotification.Identifier = TEXT("Starving");
	StarvingNotification.Message = TEXT("Starving");
	StarvingNotification.Icon = HungerIcon;

	AddNotification(StarvingNotification);
}

void UNotificationPanelWidget::RemoveStarvingNotification(const float& Time)
{
	RemoveNotification(TEXT("Starving"));
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
