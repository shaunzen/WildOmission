// Copyright Telephone Studios. All Rights Reserved.

#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "WildOmission/UI/Inventory/InventoryWidget.h"
#include "WildOmission/UI/Inventory/PlayerInventoryWidget.h"
#include "WildOmission/UI/Inventory/SelectedItemWidget.h"
#include "WildOmission/UI/Inventory/HoveredItemNameTag.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Items/BuildingHammerItem.h"
#include "WildOmission/UI/Crafting/CraftingMenuWidget.h"
#include "WildOmission/Components/InteractionComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "VitalsWidget.h"
#include "WildOmission/Deployables/ItemContainerBase.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bInventoryMenuOpen = false;
	bCraftingMenuOpen = false;
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuBackgroundBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("MenuBackgroundMouseButtonDown"));
	
	UPlayerInventoryComponent* PlayerInventoryComponent = GetOwningPlayerPawn<APawn>()->FindComponentByClass<UPlayerInventoryComponent>();
	UInventoryManipulatorComponent* PlayerInventoryManipulatorComponent = GetOwningPlayerPawn<APawn>()->FindComponentByClass<UInventoryManipulatorComponent>();
	if (PlayerInventoryComponent == nullptr || PlayerInventoryManipulatorComponent == nullptr)
	{
		return;
	}

	PlayerInventory->Setup(this, PlayerInventoryComponent);

	PlayerInventory->Refresh();

	SelectedItem->Hide();
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInteractionPrompt();
	UpdateDurabilityPrompt();
	UpdateFollowMousePointerWidgets();
}

bool UPlayerHUDWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (Success == false)
	{
		return false;
	}

	UpdateBrandingText();

	return true;
}

void UPlayerHUDWidget::OpenContainer(AItemContainerBase* Container)
{
	OpenMenuPanel();
	SwitchToInventoryMenu();

	if (OpenContainerWidget != nullptr || Container->GetWidgetClass() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot open container. Container already open, or Widget class not defined."));
		return;
	}

	OpenContainerWidget = CreateWidget<UInventoryWidget>(this, Container->GetWidgetClass(), FName("OpenContainer"));

	OpenContainerWidget->Setup(this, Container->GetInventoryComponent());
	OpenContainerWidget->CreateSlots();
	OpenContainerWidget->Open();

	UHorizontalBoxSlot* OpenContainerSlot = InventoryHorizontalBox->AddChildToHorizontalBox(OpenContainerWidget);
	if (OpenContainerSlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to open container, couldn't get container slot"));
		return;
	}
	
	FMargin ContainerWidgetPadding;
	ContainerWidgetPadding.Left = 10.0f;
	
	OpenContainerSlot->SetPadding(ContainerWidgetPadding);
	OpenContainerSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
	OpenContainerSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
}

void UPlayerHUDWidget::ToggleInventoryMenu()
{
	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UInventoryManipulatorComponent* OwnerInventoryManipulator = OwnerPawn->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulator == nullptr)
	{
		return;
	}

	if (!IsMenuOpen())
	{
		OpenMenuPanel();
		SwitchToInventoryMenu();
	}
	else if (IsCraftingMenuOpen())
	{
		SwitchToInventoryMenu();
	}
	else if (IsInventoryMenuOpen())
	{
		HoveredItemNameTag->Hide();
		OwnerInventoryManipulator->Server_DropSelectedItemInWorld(false);
		CloseMenuPanel();
	}
}

void UPlayerHUDWidget::ToggleCraftingMenu()
{
	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UInventoryManipulatorComponent* OwnerInventoryManipulator = OwnerPawn->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulator == nullptr)
	{
		return;
	}

	if (!IsMenuOpen())
	{
		OpenMenuPanel();
		SwitchToCraftingMenu();
	}
	else if (IsInventoryMenuOpen())
	{
		HoveredItemNameTag->Hide();
		OwnerInventoryManipulator->Server_DropSelectedItemInWorld(false);
		SwitchToCraftingMenu();
	}
	else if (IsCraftingMenuOpen())
	{
		SwitchToInventoryMenu();
		CloseMenuPanel();
	}
}

bool UPlayerHUDWidget::IsMenuOpen() const
{
	return bInventoryMenuOpen || bCraftingMenuOpen;
}

bool UPlayerHUDWidget::IsInventoryMenuOpen() const
{
	return bInventoryMenuOpen;
}

bool UPlayerHUDWidget::IsCraftingMenuOpen() const
{
	return bCraftingMenuOpen;
}

UPlayerInventoryWidget* UPlayerHUDWidget::GetPlayerInventoryWidget()
{
	return PlayerInventory;
}

UHoveredItemNameTag* UPlayerHUDWidget::GetHoveredItemNameTag() const
{
	return HoveredItemNameTag;
}

bool UPlayerHUDWidget::SelectedItemVisible() const
{
	return SelectedItem->GetVisibility() == ESlateVisibility::HitTestInvisible;
}

void UPlayerHUDWidget::UpdateBrandingText()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		return;
	}

	FString BrandingString = FString::Printf(TEXT("Wild Omission %s"), *GameInstance->GetVersion());
	BrandingTextBlock->SetText(FText::FromString(BrandingString));
}

void UPlayerHUDWidget::OpenMenuPanel()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
	MenuBackgroundBorder->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerHUDWidget::SwitchToInventoryMenu()
{
	bInventoryMenuOpen = true;
	bCraftingMenuOpen = false;

	MenuSwitcher->SetActiveWidget(InventoryHorizontalBox);

	PlayerInventory->Open();

	CraftingMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::SwitchToCraftingMenu()
{
	bInventoryMenuOpen = false;
	bCraftingMenuOpen = true;

	MenuSwitcher->SetActiveWidget(CraftingMenu);
	CraftingMenu->Refresh();

	PlayerInventory->Close();

	CraftingMenu->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerHUDWidget::CloseMenuPanel()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
	
	bInventoryMenuOpen = false;
	bCraftingMenuOpen = false;
	
	PlayerInventory->Close();
	
	if (OpenContainerWidget != nullptr)
	{
		if (AItemContainerBase* OpenedContainer = Cast<AItemContainerBase>(OpenContainerWidget->GetInventoryComponent()->GetOwner()))
		{
			OpenedContainer->Server_UnOccupy();
		}

		OpenContainerWidget->Close();
		OpenContainerWidget->RemoveFromParent();
		OpenContainerWidget = nullptr;
	}
	
	CraftingMenu->SetVisibility(ESlateVisibility::Hidden);

	MenuBackgroundBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::UpdateInteractionPrompt()
{
	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}
	
	UInteractionComponent* OwnerInteractionComopnent = OwnerPawn->FindComponentByClass<UInteractionComponent>();
	if (OwnerInteractionComopnent == nullptr)
	{
		return;
	}

	if (!IsMenuOpen())
	{
		InteractionPrompt->SetText(FText::FromString(OwnerInteractionComopnent->GetInteractionString()));
	}
	else
	{
		InteractionPrompt->SetText(FText::FromString(FString("")));
	}
}

void UPlayerHUDWidget::UpdateDurabilityPrompt()
{
	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = OwnerPawn->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	float DurabilityPercent = 0.0f;
	ABuildingHammerItem* HeldBuildingHammer = Cast<ABuildingHammerItem>(OwnerEquipComponent->GetEquipedItem());
	if (HeldBuildingHammer && HeldBuildingHammer->GetLookingAtItemDurability(DurabilityPercent) && !IsMenuOpen())
	{
		DurabilityBar->SetVisibility(ESlateVisibility::Visible);
		DurabilityBar->SetPercent(DurabilityPercent);
		return;
	}

	DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::UpdateFollowMousePointerWidgets()
{	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UCanvasPanelSlot* SelectedItemSlot = Cast<UCanvasPanelSlot>(SelectedItem->Slot);
	UCanvasPanelSlot* HoveredItemSlot = Cast<UCanvasPanelSlot>(HoveredItemNameTag->Slot);
	if (PlayerController == nullptr || SelectedItemSlot == nullptr || HoveredItemSlot == nullptr|| GEngine->GameViewport == 0)
	{
		return;
	}

	// Get mouse position
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	// Get viewport size
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// Normalize the mouse position
	FVector2D NormalizedMousePosition;
	NormalizedMousePosition = MousePosition / ViewportSize;

	// Create anchor from destination
	FAnchors SelectedItemAnchor;
	SelectedItemAnchor.Minimum.X = NormalizedMousePosition.X;
	SelectedItemAnchor.Minimum.Y = NormalizedMousePosition.Y;
	SelectedItemAnchor.Maximum.X = NormalizedMousePosition.X;
	SelectedItemAnchor.Maximum.Y = NormalizedMousePosition.Y;


	FAnchors HoveredItemAnchor;
	HoveredItemAnchor.Minimum.X = NormalizedMousePosition.X;
	HoveredItemAnchor.Minimum.Y = NormalizedMousePosition.Y - (NormalizedMousePosition.Y * 0.03);
	HoveredItemAnchor.Maximum.X = NormalizedMousePosition.X;
	HoveredItemAnchor.Maximum.Y = NormalizedMousePosition.Y - (NormalizedMousePosition.Y * 0.03);

	// Update slot anchor and position
	SelectedItemSlot->SetAnchors(SelectedItemAnchor);
	SelectedItemSlot->SetPosition(FVector2D::ZeroVector);
	HoveredItemSlot->SetAnchors(HoveredItemAnchor);
	HoveredItemSlot->SetPosition(FVector2D::ZeroVector);
}

void UPlayerHUDWidget::MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UInventoryManipulatorComponent* OwnerInventoryManipulatorComponent = OwnerPawn->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulatorComponent == nullptr)
	{
		return;
	}

	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OwnerInventoryManipulatorComponent->Server_DropSelectedItemInWorld(false);
	}
	else if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OwnerInventoryManipulatorComponent->Server_DropSelectedItemInWorld(true);
	}
}