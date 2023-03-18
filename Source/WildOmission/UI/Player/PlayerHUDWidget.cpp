// Copyright Telephone Studios. All Rights Reserved.

#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "WildOmission/UI/Inventory/InventoryWidget.h"
#include "WildOmission/UI/Inventory/PlayerInventoryWidget.h"
#include "WildOmission/UI/Inventory/SelectedItemWidget.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/UI/Crafting/CraftingWidget.h"
#include "WildOmission/Components/InteractionComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "VitalsWidget.h"

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
	if (PlayerInventoryComponent == nullptr)
	{
		return;
	}

	PlayerInventory->Setup(PlayerInventoryComponent);

	RefreshInventoryStates();
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInteractionPrompt();
	UpdateSelectedItemLocation();
}

void UPlayerHUDWidget::RefreshInventoryStates()
{
	// TODO all open inventory menus refresh

	UInventoryManipulatorComponent* PlayerInventoryManipulator = PlayerInventory->GetInventoryComponent()->GetManipulator();
	if (PlayerInventoryManipulator == nullptr)
	{
		return;
	}
	
	PlayerInventory->Refresh();
	
	if (PlayerInventoryManipulator->IsDragging())
	{
		SelectedItem->Show();
		FItemData* SelectedItemData = PlayerInventory->GetInventoryComponent()->GetItemData(PlayerInventoryManipulator->GetSelectedItem().Name);
		SelectedItem->SetItem(SelectedItemData->Thumbnail, PlayerInventoryManipulator->GetSelectedItem().Quantity);
	}
	else
	{
		SelectedItem->Hide();
	}
}

void UPlayerHUDWidget::ToggleInventoryMenu()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Toggle Inventory Menu"));
}

void UPlayerHUDWidget::ToggleCraftingMenu()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Toggle Crafting Menu"));
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

void UPlayerHUDWidget::UpdateInteractionPrompt()
{
	UInteractionComponent* OwnerInteractionComopnent = GetOwningPlayerPawn<APawn>()->FindComponentByClass<UInteractionComponent>();
	if (OwnerInteractionComopnent == nullptr)
	{
		return;
	}

	if (!bInventoryMenuOpen)
	{
		InteractionPrompt->SetText(FText::FromString(OwnerInteractionComopnent->GetInteractionString()));
	}
	else
	{
		InteractionPrompt->SetText(FText::FromString(FString("")));
	}
}

void UPlayerHUDWidget::UpdateSelectedItemLocation()
{	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UCanvasPanelSlot* SelectedItemSlot = Cast<UCanvasPanelSlot>(SelectedItem->Slot);
	if (PlayerController == nullptr || SelectedItemSlot == nullptr || GEngine->GameViewport == 0)
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
	FAnchors NewAnchor;
	NewAnchor.Minimum.X = NormalizedMousePosition.X;
	NewAnchor.Minimum.Y = NormalizedMousePosition.Y;
	NewAnchor.Maximum.X = NormalizedMousePosition.X;
	NewAnchor.Maximum.Y = NormalizedMousePosition.Y;

	// Update slot anchor and position
	SelectedItemSlot->SetAnchors(NewAnchor);
	SelectedItemSlot->SetPosition(FVector2D::ZeroVector);
}

void UPlayerHUDWidget::MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	UInventoryManipulatorComponent* PlayerInventoryManipulator = PlayerInventory->GetInventoryComponent()->GetManipulator();
	if (PlayerInventoryManipulator == nullptr)
	{
		return;
	}

	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		PlayerInventoryManipulator->Server_DropSelectedItemInWorld(false);
	}
	else if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		PlayerInventoryManipulator->Server_DropSelectedItemInWorld(true);
	}

	RefreshInventoryStates();
}