// Copyright Telephone Studios. All Rights Reserved.


#include "UI/InventoryMenuWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/PlayerInventoryWidget.h"
#include "UI/SelectedItemWidget.h"
#include "UI/HoveredItemNameTag.h"
#include "Components/PlayerInventoryComponent.h"
#include "Interfaces/ItemContainer.h"
#include "Log.h"

void UInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UPlayerInventoryComponent* PlayerInventoryComponent = GetOwningPlayerPawn<APawn>()->FindComponentByClass<UPlayerInventoryComponent>();
	if (PlayerInventoryComponent == nullptr)
	{
		return;
	}

	PlayerInventoryWidget->Setup(this, PlayerInventoryComponent);
	PlayerInventoryWidget->Refresh();
	SelectedItemWidget->Hide();
}

void UInventoryMenuWidget::NativeTick(const FGeometry& MyGeomentry, float InDeltaTime)
{
	Super::NativeTick(MyGeomentry, InDeltaTime);

	UpdateFollowMousePointerWidgets();
}

void UInventoryMenuWidget::OnOpen()
{
	PlayerInventoryWidget->Open();
}

void UInventoryMenuWidget::OnClose(bool ShouldCloseContainer)
{
	PlayerInventoryWidget->Close();

	if (OpenContainerWidget != nullptr)
	{
		if (IItemContainer* OpenedContainer = Cast<IItemContainer>(OpenContainerWidget->GetInventoryComponent()->GetOwner()))
		{
			OpenedContainer->UnOccupy();
		}

		OpenContainerWidget->Close();
		OpenContainerWidget->RemoveFromParent();
		OpenContainerWidget = nullptr;
	}

}

void UInventoryMenuWidget::OpenContainer(UInventoryComponent* ContainerInventoryComponent, UClass* ContainerWidgetClass)
{
	if (OpenContainerWidget || ContainerWidgetClass == nullptr)
	{
		UE_LOG(LogInventory, Display, TEXT("This container cannot be opened, an existing container is already open.")).
		return;
	}

	OpenContainerWidget = CreateWidget<UInventoryWidget>(this, ContainerWidgetClass);

	OpenContainerWidget->Setup(nullptr, ContainerInventoryComponent);
	OpenContainerWidget->CreateSlots();
	OpenContainerWidget->Open();

	UHorizontalBoxSlot* OpenContainerSlot = InventoryHorizontalBox->AddChildToHorizontalBox(OpenContainerWidget);
	if (OpenContainerSlot == nullptr)
	{
		UE_LOG(LogInventory, Warning, TEXT("Couldn't open container, owning slot returned a nullptr."));
		return;
	}

	FMargin ContainerWidgetPadding;
	ContainerWidgetPadding.Left = 10.0f;

	OpenContainerSlot->SetPadding(ContainerWidgetPadding);
	OpenContainerSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
	OpenContainerSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
}

UPlayerInventoryWidget* UInventoryMenuWidget::GetPlayerInventoryWidget() const
{
	return PlayerInventoryWidget;
}

USelectedItemWidget* UInventoryMenuWidget::GetSelectedItemWidget() const
{
	return SelectedItemWidget;
}

UHoveredItemNameTag* UInventoryMenuWidget::GetHoveredItemNameTagWidget() const
{
	return HoveredItemNameTagWidget;
}

bool UInventoryMenuWidget::SelectedItemVisible() const
{
	return SelectedItemWidget->GetVisibility() == ESlateVisibility::HitTestInvisible;
}

void UInventoryMenuWidget::UpdateFollowMousePointerWidgets()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UCanvasPanelSlot* SelectedItemWidgetSlot = Cast<UCanvasPanelSlot>(SelectedItemWidget->Slot);
	UCanvasPanelSlot* HoveredItemWidgetSlot = Cast<UCanvasPanelSlot>(HoveredItemNameTagWidget->Slot);
	if (PlayerController == nullptr || SelectedItemWidgetSlot == nullptr || HoveredItemWidgetSlot == nullptr || GEngine->GameViewport == 0)
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
	SelectedItemWidgetSlot->SetAnchors(SelectedItemAnchor);
	SelectedItemWidgetSlot->SetPosition(FVector2D::ZeroVector);
	HoveredItemWidgetSlot->SetAnchors(HoveredItemAnchor);
	HoveredItemWidgetSlot->SetPosition(FVector2D::ZeroVector);
}
