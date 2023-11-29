// Copyright Telephone Studios. All Rights Reserved.


#include "UI/NotificationPanelWidget.h"
#include "NotificationWidget.h"
#include "Components/PanelWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Structs/Notification.h"
#include "Components/InventoryComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/BuilderComponent.h"
#include "UObject/ConstructorHelpers.h"

static UMaterialInterface* HealthIcon = nullptr;
static UMaterialInterface* ThirstIcon = nullptr;
static UMaterialInterface* HungerIcon = nullptr;
static UMaterialInterface* BuildingIcon = nullptr;

const static FName THIRSTY_IDENTIFIER = TEXT("Thirsty");
const static FName STARVING_IDENTIFIER = TEXT("Starving");
const static FName BUILDING_PRIVILEGE_IDENTIFIER = TEXT("BuildingPrivilege");
const static FName BUILDING_BLOCKED_IDENTIFIER = TEXT("BuildingBlocked");

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

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BuildingIconObject(TEXT("/Game/WildOmissionCore/UI/Icons/Generic/M_Building_Icon_Inst"));
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
	if (UBuilderComponent* OwnerBuilderComponent = OwnerPawn->FindComponentByClass<UBuilderComponent>())
	{
		// TODO bind delegates
		OwnerBuilderComponent->OnAddBuildingPrivilegeNotification.AddDynamic(this, &UNotificationPanelWidget::AddBuildingPrivilegeNotification);
		OwnerBuilderComponent->OnClearBuildingPrivilegeNotification.AddDynamic(this, &UNotificationPanelWidget::ClearBuildingPrivilegeNotification);
	}
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
	ThirstyNotification.Identifier = THIRSTY_IDENTIFIER;
	ThirstyNotification.Message = TEXT("Thirsty");
	ThirstyNotification.Icon = ThirstIcon;

	AddNotification(ThirstyNotification);
}

void UNotificationPanelWidget::RemoveThirstyNotification(const float& Time)
{
	RemoveNotification(THIRSTY_IDENTIFIER);
}

void UNotificationPanelWidget::AddStarvingNotification(const float& Time)
{
	FNotification StarvingNotification;
	StarvingNotification.Negative = true;
	StarvingNotification.Time = Time;
	StarvingNotification.Duration = 0.0f;
	StarvingNotification.Identifier = STARVING_IDENTIFIER;
	StarvingNotification.Message = TEXT("Starving");
	StarvingNotification.Icon = HungerIcon;

	AddNotification(StarvingNotification);
}

void UNotificationPanelWidget::RemoveStarvingNotification(const float& Time)
{
	RemoveNotification(STARVING_IDENTIFIER);
}

void UNotificationPanelWidget::AddBuildingPrivilegeNotification(bool HasBuildingPrivilege)
{
	const FName Identifier = HasBuildingPrivilege ? BUILDING_PRIVILEGE_IDENTIFIER : BUILDING_BLOCKED_IDENTIFIER;
	const FName OtherIdentifier = HasBuildingPrivilege ? BUILDING_BLOCKED_IDENTIFIER : BUILDING_PRIVILEGE_IDENTIFIER;

	if (HasNotification(Identifier))
	{
		return;
	}

	if (HasNotification(OtherIdentifier))
	{
		RemoveNotification(OtherIdentifier);
	}

	FNotification BuildingNotification;
	BuildingNotification.Negative = !HasBuildingPrivilege;
	BuildingNotification.Positive = HasBuildingPrivilege;
	BuildingNotification.Time = GetWorld()->GetRealTimeSeconds();
	BuildingNotification.Duration = 0.0f;
	BuildingNotification.Identifier = Identifier;
	BuildingNotification.Message = HasBuildingPrivilege ? TEXT("Building Privilege") : TEXT("Building Blocked");
	BuildingNotification.Icon = BuildingIcon;

	AddNotification(BuildingNotification);
}

void UNotificationPanelWidget::ClearBuildingPrivilegeNotification()
{
	RemoveNotification(BUILDING_PRIVILEGE_IDENTIFIER);
	RemoveNotification(BUILDING_BLOCKED_IDENTIFIER);
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

bool UNotificationPanelWidget::HasNotification(const FName& NotificationIdentifier)
{
	for (UWidget* ChildWidget : StatusNotificationContainer->GetAllChildren())
	{
		UNotificationWidget* ChildNotificationWidget = Cast<UNotificationWidget>(ChildWidget);
		if (ChildNotificationWidget == nullptr || ChildNotificationWidget->GetNotification().Identifier != NotificationIdentifier)
		{
			continue;
		}

		return true;
	}

	for (UWidget* ChildWidget : NotificationContainer->GetAllChildren())
	{
		UNotificationWidget* ChildNotificationWidget = Cast<UNotificationWidget>(ChildWidget);
		if (ChildNotificationWidget == nullptr || ChildNotificationWidget->GetNotification().Identifier != NotificationIdentifier)
		{
			continue;
		}

		return true;
	}

	return false;
}

void UNotificationPanelWidget::RemoveNotification(const FName& NotificationIdentifier)
{
	for (UWidget* ChildWidget : StatusNotificationContainer->GetAllChildren())
	{
		UNotificationWidget* ChildNotificationWidget = Cast<UNotificationWidget>(ChildWidget);
		if (ChildNotificationWidget == nullptr || ChildNotificationWidget->GetNotification().Identifier != NotificationIdentifier || ChildNotificationWidget->IsSlidingOut())
		{
			continue;
		}

		ChildNotificationWidget->HandleRemoval();
		return;
	}

	for (UWidget* ChildWidget : NotificationContainer->GetAllChildren())
	{
		UNotificationWidget* ChildNotificationWidget = Cast<UNotificationWidget>(ChildWidget);
		if (ChildNotificationWidget == nullptr || ChildNotificationWidget->GetNotification().Identifier != NotificationIdentifier || ChildNotificationWidget->IsSlidingOut())
		{
			continue;
		}

		ChildNotificationWidget->HandleRemoval();
		return;
	}
}
