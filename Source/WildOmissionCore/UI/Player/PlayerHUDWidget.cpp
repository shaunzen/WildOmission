// Copyright Telephone Studios. All Rights Reserved.

#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "UI/InventoryWidget.h"
#include "UI/PlayerInventoryWidget.h"
#include "UI/SelectedItemWidget.h"
#include "UI/HoveredItemNameTag.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Items/BuildingHammerItem.h"
#include "UI/CraftingMenuWidget.h"
#include "UI/GameChatWidget.h"
#include "Interfaces/ChatMessageContainer.h"
#include "Interfaces/ChatMessageSender.h"
#include "UI/NotificationPanelWidget.h"
#include "Components/InteractionComponent.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "UI/VitalsWidget.h"
#include "Deployables/ItemContainerBase.h"
#include "GameFramework/GameState.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bInventoryMenuOpen = false;
	bCraftingMenuOpen = false;
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuBackgroundBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("MenuBackgroundMouseButtonDown"));
	
	Chat->Setup(this, Cast<IChatMessageContainer>(GetWorld()->GetGameState()));
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInteractionPrompt();
	UpdateDurabilityPrompt();
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

void UPlayerHUDWidget::ToggleInventoryMenu(bool ForceOpen)
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
		HoveredItemNameTag->Hide();
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

	if (!IsMenuOpen() || ForceOpen == true)
	{
		OpenMenuPanel();
		SwitchToCraftingMenu();
	}
	else if (IsInventoryMenuOpen())
	{
		HoveredItemNameTag->Hide();
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
	return bInventoryMenuOpen || bCraftingMenuOpen || IsChatMenuOpen();
}

bool UPlayerHUDWidget::IsInventoryMenuOpen() const
{
	return bInventoryMenuOpen;
}

bool UPlayerHUDWidget::IsCraftingMenuOpen() const
{
	return bCraftingMenuOpen;
}

bool UPlayerHUDWidget::IsChatMenuOpen() const
{
	return Chat->IsOpen();
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