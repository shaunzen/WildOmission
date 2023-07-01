// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "UI/VitalsWidget.h"
#include "UI/InventoryMenuWidget.h"
#include "UI/CraftingMenuWidget.h"
#include "UI/GameChatWidget.h"
#include "UI/NotificationPanelWidget.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Interfaces/ChatMessageContainer.h"
#include "Interfaces/ChatMessageSender.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "GameFramework/GameState.h"

// TODO these are to be removed in future versions
#include "Items/BuildingHammerItem.h"
#include "Components/EquipComponent.h"
#include "Components/ProgressBar.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	InteractionPrompt = nullptr;
	DurabilityBar = nullptr;
	MenuBackgroundBorder = nullptr;
	MenuSwitcher = nullptr;
	InventoryMenu = nullptr;
	CraftingMenu = nullptr;
	Chat = nullptr;
	NotificationPanel = nullptr;
	Vitals = nullptr;
	BrandingTextBlock = nullptr;
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateBrandingText();
	MenuBackgroundBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("MenuBackgroundMouseButtonDown"));
	Chat->Setup(this, Cast<IChatMessageContainer>(GetWorld()->GetGameState()));
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInteractionPrompt();
	UpdateDurabilityPrompt();
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
		Chat->Open(Cast<IChatMessageSender>(GetOwningPlayer()));
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
	return MenuSwitcher->GetActiveWidget() == CraftingMenu;
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
	MenuSwitcher->SetActiveWidget(InventoryMenu);
	InventoryMenu->Open();
}

void UPlayerHUDWidget::SwitchToCraftingMenu()
{
	InventoryMenu->Close();
	
	MenuSwitcher->SetActiveWidget(CraftingMenu);
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