// Copyright Telephone Studios. All Rights Reserved.


#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Items/BuildingHammerItem.h"
#include "Deployables/BuildingBlock.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Log.h"

void UBuildingHammerWidget::Setup(ABuildingHammerItem* BuildingHammer, ADeployable* InDeployable)
{
	OwnerBuildingHammer = BuildingHammer;
	Deployable = InDeployable;
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	this->AddToViewport();
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
	SetMouseCursorToCenter();
	this->SetFocus();
	
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	if (BuildingBlock && BuildingBlock->IsUpgradable())
	{
		SetupUpgradeText();
	}
	else
	{
		UpgradeTextBlock->SetText(FText::FromString(TEXT("Not Upgradable")));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		UpgradeHasTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetupDestroyText();
}

void UBuildingHammerWidget::Teardown()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}
	
	if (OnTeardown.IsBound())
	{
		OnTeardown.Broadcast();
	}
	this->RemoveFromParent();
}

void UBuildingHammerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	bool UpgradeAlreadySelected = UpgradeSelected;
	bool DestroyAlreadySelected = DestroySelected;
	bool CanUpgradeIfUpgradeable = false;
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	if (BuildingBlock)
	{ 
		CanUpgradeIfUpgradeable = CanPlayerAffordUpgrade();
	}

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	float MousePositionX = 0;
	float MousePositionY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	PlayerController->GetMousePosition(MousePositionX, MousePositionY);
	
	const int32 ViewportCenterX = ViewportSizeX * 0.5f;
	const int32 ViewportCenterY = ViewportSizeY * 0.5f;
	
	UpgradeSelected = FMath::RoundToInt32(MousePositionX) > (ViewportCenterX + 100);
	ScaleWidgetByBool(UpgradePanel, UpgradeSelected && CanUpgradeIfUpgradeable);

	DestroySelected = FMath::RoundToInt32(MousePositionX) < (ViewportCenterX - 100);
	ScaleWidgetByBool(DestroyPanel, DestroySelected);

	if (ButtonSound &&
		(UpgradeSelected && !UpgradeAlreadySelected && CanUpgradeIfUpgradeable) || (DestroySelected && !DestroyAlreadySelected))
	{
		PlaySound(ButtonSound);
	}
}

FReply UBuildingHammerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	if (BuildingBlock && OwnerBuildingHammer && UpgradeSelected && CanPlayerAffordUpgrade())
	{
		OwnerBuildingHammer->Server_UpgradeCurrentDeployable();
	}
	else if (OwnerBuildingHammer && DestroySelected)
	{
		OwnerBuildingHammer->Server_DestroyCurrentDeployable();
	}

	this->Teardown();

	return FReply::Handled();
}

void UBuildingHammerWidget::ScaleWidgetByBool(UWidget* WidgetToScale, bool Increase)
{
	float ScaleAmount = Increase ? 2.0f : -2.0f;

	FVector2D OldRenderScale = WidgetToScale->GetRenderTransform().Scale;
	FVector2D NewRenderScale = FVector2D(FMath::Clamp(OldRenderScale.X + ScaleAmount * GetWorld()->GetDeltaSeconds(), 1.0f, 1.1f),
		FMath::Clamp(OldRenderScale.Y + ScaleAmount * GetWorld()->GetDeltaSeconds(), 1.0f, 1.1f));
	WidgetToScale->SetRenderScale(NewRenderScale);
}

bool UBuildingHammerWidget::CanPlayerAffordUpgrade() const
{
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (BuildingBlock == nullptr || OwnerInventoryComponent == nullptr)
	{
		return false;
	}

	FInventoryItem UpgradeCost = ABuildingHammerItem::GetUpgradeCostForBuildingBlock(BuildingBlock);
	if (OwnerInventoryComponent->GetContents()->GetItemQuantity(UpgradeCost.Name) < UpgradeCost.Quantity)
	{
		return false;
	}

	return true;
}

void UBuildingHammerWidget::SetupUpgradeText()
{
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	if (BuildingBlock == nullptr || GetOwningPlayerPawn() == nullptr)
	{
		UpgradeTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeCostTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeHasTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetIsEnabled(false);
		UpgradeHasTextBlock->SetIsEnabled(false);
		return;
	}

	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		UpgradeTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeCostTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeHasTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetIsEnabled(false);
		UpgradeHasTextBlock->SetIsEnabled(false);
		return;
	}

	FInventoryItem UpgradeCost = ABuildingHammerItem::GetUpgradeCostForBuildingBlock(BuildingBlock);

	FItemData* ItemData = UInventoryComponent::GetItemData(UpgradeCost.Name);
	if (ItemData == nullptr)
	{
		UE_LOG(LogDeployables, Warning, TEXT("Item Data for '%s' wasn't availible."), *UpgradeCost.Name.ToString());
		UpgradeTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeCostTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeHasTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetIsEnabled(false);
		UpgradeHasTextBlock->SetIsEnabled(false);
		return;
	}

	UpgradeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Upgrade To %s"), *ItemData->DisplayName)));
	UpgradeCostTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Requires %i %s"), UpgradeCost.Quantity, *ItemData->DisplayName)));
	UpgradeHasTextBlock->SetText(FText::FromString(FString::Printf(TEXT("You Have %i %s"),
		OwnerInventoryComponent->GetContents()->GetItemQuantity(UpgradeCost.Name), *ItemData->DisplayName)));

	if (!CanPlayerAffordUpgrade())
	{
		UpgradeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Cannot Upgrade to %s"), *ItemData->DisplayName)));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetIsEnabled(false);
		UpgradeHasTextBlock->SetIsEnabled(false);
	}
}

void UBuildingHammerWidget::SetupDestroyText()
{
	FInventoryItem RefundItem = ABuildingHammerItem::GetDestructionRefundForDeployable(Deployable);
	FItemData* RefundItemData = UInventoryComponent::GetItemData(RefundItem.Name);
	if (RefundItemData == nullptr)
	{
		DestroyRefundTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	DestroyRefundTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Refund %i %s"), RefundItem.Quantity, *RefundItemData->DisplayName)));
}

void UBuildingHammerWidget::SetMouseCursorToCenter()
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
