// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/InventoryMenuWidget.h"
#include "UI/CraftingMenuWidget.h"
#include "UI/GameChatWidget.h"
#include "UI/NotificationPanelWidget.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "GameFramework/GameState.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	MenuBackgroundBorder = nullptr;
	MenuSwitcher = nullptr;
	OpenCraftingButton = nullptr;
	InventoryMenu = nullptr;
	OpenInventoryButton = nullptr;
	CraftingMenu = nullptr;
	Chat = nullptr;
	NotificationPanel = nullptr;
	BrandingTextBlock = nullptr;
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateBrandingText();

	MenuBackgroundBorder->OnMouseButtonDownEvent.BindUFunction(this, TEXT("MenuBackgroundMouseButtonDown"));
	Chat->OnToggleRequested.AddDynamic(this, &UPlayerHUDWidget::ToggleChatMenu);

	OpenCraftingButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::SwitchToCraftingMenu);
	OpenCraftingButton->SetVisibility(ESlateVisibility::Hidden);
	OpenInventoryButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::SwitchToInventoryMenu);

	UInventoryManipulatorComponent* PlayerInventoryManipulatorComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryManipulatorComponent>();
	if (PlayerInventoryManipulatorComponent == nullptr)
	{
		return;
	}
	PlayerInventoryManipulatorComponent->OnOpenContainerChanged.AddDynamic(this, &UPlayerHUDWidget::OnOpenContainerChanged);
}

void UPlayerHUDWidget::ShowBranding(bool Show)
{
	BrandingTextBlock->SetVisibility(Show ? ESlateVisibility::Visible: ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::ShowCrosshair(bool Show)
{
	Crosshair->SetVisibility(Show ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::SetHideChatUnlessOpen(bool HideChatUnlessOpen)
{
	Chat->SetHideUnlessOpen(HideChatUnlessOpen);
}

void UPlayerHUDWidget::ToggleInventoryMenu(bool ForceOpen)
{
	if (!IsMenuOpen() || ForceOpen == true)
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

		OwnerInventoryManipulator->DropSelectedItemInWorld(false);
		OpenCraftingButton->SetVisibility(ESlateVisibility::Hidden);
		CloseMenuPanel();
	}
	else
	{
		Chat->Close();
		SwitchToInventoryMenu();
	}
}

void UPlayerHUDWidget::ToggleCraftingMenu(bool ForceOpen)
{
	if (!IsMenuOpen() || ForceOpen == true)
	{
		OpenMenuPanel();
		SwitchToCraftingMenu();
	}
	else if (IsInventoryMenuOpen())
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

		OwnerInventoryManipulator->DropSelectedItemInWorld(false);
		SwitchToCraftingMenu();
	}
	else if (IsCraftingMenuOpen())
	{
		SwitchToInventoryMenu();
		CloseMenuPanel();
	}
	else
	{
		Chat->Close();
		SwitchToCraftingMenu();
	}
}

void UPlayerHUDWidget::ToggleChatMenu()
{
	if (!IsMenuOpen())
	{
		OpenMenuPanel(false);
		Chat->Open();
	}
	else if (Chat->IsOpen())
	{
		Chat->Close();
		CloseMenuPanel();
	}
}

bool UPlayerHUDWidget::IsMenuOpen() const
{
	return IsInventoryMenuOpen() || IsCraftingMenuOpen() || IsChatMenuOpen();
}

bool UPlayerHUDWidget::IsInventoryMenuOpen() const
{
	return InventoryMenu->IsOpen();
}

bool UPlayerHUDWidget::IsCraftingMenuOpen() const
{
	return MenuSwitcher->GetActiveWidget() == CraftingPanel;
}

bool UPlayerHUDWidget::IsChatMenuOpen() const
{
	return Chat->IsOpen();
}

UInventoryMenuWidget* UPlayerHUDWidget::GetInventoryMenu() const
{
	return InventoryMenu;
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

void UPlayerHUDWidget::OpenMenuPanel(bool ShowBackground)
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
	SetMouseCursorToCenter();

	if (ShowBackground)
	{
		MenuBackgroundBorder->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerHUDWidget::SwitchToInventoryMenu()
{
	MenuSwitcher->SetActiveWidget(InventoryPanel);
	OpenCraftingButton->SetVisibility(ESlateVisibility::Visible);
	InventoryMenu->Open();
}

void UPlayerHUDWidget::SwitchToCraftingMenu()
{
	InventoryMenu->Close();
	MenuSwitcher->SetActiveWidget(CraftingPanel);
	CraftingMenu->Refresh();
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
	
	InventoryMenu->Close(true);
	OpenCraftingButton->SetVisibility(ESlateVisibility::Hidden);
	MenuBackgroundBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUDWidget::SetMouseCursorToCenter()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	PlayerController->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);
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
		OwnerInventoryManipulatorComponent->DropSelectedItemInWorld(false);
	}
	else if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OwnerInventoryManipulatorComponent->DropSelectedItemInWorld(true);
	}
}

void UPlayerHUDWidget::OnOpenContainerChanged(UInventoryComponent* OpenContainer)
{
	if (OpenContainer == nullptr)
	{
		return;
	}

	ToggleInventoryMenu(true);
}
