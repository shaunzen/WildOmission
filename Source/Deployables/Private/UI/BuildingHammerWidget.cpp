// Copyright Telephone Studios. All Rights Reserved.


#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Items/BuildingHammerItem.h"
#include "Deployables/BuildingBlock.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Log.h"

void UBuildingHammerWidget::Show(ABuildingHammerItem* BuildingHammer, ADeployable* InDeployable)
{
	OwnerBuildingHammer = BuildingHammer;
	Deployable = InDeployable;
	APlayerController* PlayerController = GetOwningPlayer();
	if (Deployable == nullptr || OwnerBuildingHammer == nullptr || PlayerController == nullptr)
	{
		this->Teardown();
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

	const bool UpgradeAlreadySelected = UpgradeSelected;
	const bool DestroyAlreadySelected = DestroySelected;
	const bool RotateAlreadySelected = RotateSelected;

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

	RotateSelected = !UpgradeSelected && !DestroySelected && FMath::RoundToInt32(MousePositionY) > (ViewportCenterY + 100);
	ScaleWidgetByBool(RotatePanel, RotateSelected);

	if (ButtonSound &&
		(UpgradeSelected && !UpgradeAlreadySelected && CanUpgradeIfUpgradeable) || (DestroySelected && !DestroyAlreadySelected) || (RotateSelected && !RotateAlreadySelected))
	{
		PlaySound(ButtonSound);
	}
}

void UBuildingHammerWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	Teardown();
}

FReply UBuildingHammerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	HandleSelection();

	return FReply::Handled();
}

FReply UBuildingHammerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	HandleSelection();

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

void UBuildingHammerWidget::HandleSelection()
{
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	if (BuildingBlock && OwnerBuildingHammer && UpgradeSelected && CanPlayerAffordUpgrade())
	{
		OwnerBuildingHammer->Server_UpgradeCurrentDeployable();
	}
	else if (OwnerBuildingHammer && DestroySelected)
	{
		OwnerBuildingHammer->Server_DestroyCurrentDeployable();
	}
	else if (OwnerBuildingHammer && RotateSelected)
	{
		OwnerBuildingHammer->Server_RotateCurrentDeployable();
	}

	this->Teardown();
}

bool UBuildingHammerWidget::CanPlayerAffordUpgrade() const
{
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(Deployable);
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (BuildingBlock == nullptr || OwnerInventoryComponent == nullptr)
	{
		return false;
	}

	TArray<FInventoryItem> UpgradeCost;
	if (!ABuildingHammerItem::GetUpgradeCostForDeployable(BuildingBlock, UpgradeCost))
	{
		return false;
	}

	FInventoryContents* InventoryContents = OwnerInventoryComponent->GetContents();
	if (InventoryContents == nullptr)
	{
		return;
	}

	for (const FInventoryItem& CostItem : UpgradeCost)
	{
		if (InventoryContents->GetItemQuantity(CostItem.Name) >= CostItem.Quantity)
		{
			continue;
		}
		
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

	TArray<FInventoryItem> UpgradeCost;

	if (!ABuildingHammerItem::GetUpgradeCostForDeployable(BuildingBlock, UpgradeCost))
	{
		UpgradeTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeCostTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeHasTextBlock->SetText(FText::FromString(TEXT("Invalid")));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetIsEnabled(false);
		UpgradeHasTextBlock->SetIsEnabled(false);
		return;
	}

	FInventoryContents* InventoryContents = OwnerInventoryComponent->GetContents();
	if (InventoryContents == nullptr)
	{
		return;
	}

	FString RequiresString;
	FString YouHaveString;
	bool InitialElement = true;
	for (const FInventoryItem& CostItem : UpgradeCost)
	{
		FItemData* ItemData = UInventoryComponent::GetItemData(CostItem.Name);
		if (ItemData == nullptr)
		{
			UE_LOG(LogDeployables, Warning, TEXT("Item Data for '%s' wasn't availible."), *CostItem.Name.ToString());
			continue;
		}

		const FString Separator = InitialElement ? TEXT("") : TEXT(",");
		
		RequiresString.Append(FString::Printf(TEXT("%s %i %s"), *Separator, CostItem.Quantity, *ItemData->DisplayName));
		YouHaveString.Append(FString::Printf(TEXT("%s %i %s"), *Separator,
			InventoryContents->GetItemQuantity(CostItem.Name), *ItemData->DisplayName));
		InitialElement = false;
	}

	
	FItemData* InitialItemData = UInventoryComponent::GetItemData(UpgradeCost[0].Name);
	if (InitialItemData == nullptr)
	{
		return;
	}

	UpgradeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Upgrade To %s"), *InitialItemData->DisplayName)));
	UpgradeCostTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Requires%s"), *RequiresString)));
	UpgradeHasTextBlock->SetText(FText::FromString(FString::Printf(TEXT("You Have%s"), *YouHaveString)));


	if (!CanPlayerAffordUpgrade())
	{
		UpgradeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Cannot Upgrade to %s"), *InitialItemData->DisplayName)));
		UpgradeTextBlock->SetIsEnabled(false);
		UpgradeCostTextBlock->SetIsEnabled(false);
		UpgradeHasTextBlock->SetIsEnabled(false);
	}
}

void UBuildingHammerWidget::SetupDestroyText()
{
	TArray<FInventoryItem> DestructionRefund;
	if (!ABuildingHammerItem::GetDestructionRefundForDeployable(Deployable, DestructionRefund))
	{
		DestroyRefundTextBlock->SetText(FText::FromString(TEXT("No Refund")));
		return;
	}

	FString RefundString;
	bool InitialElement = true;
	for (const FInventoryItem& RefundItem : DestructionRefund)
	{
		FItemData* RefundItemData = UInventoryComponent::GetItemData(RefundItem.Name);
		if (RefundItemData == nullptr)
		{
			continue;
		}

		const FString Separator = InitialElement ? TEXT("") : TEXT(",");
		RefundString.Append(FString::Printf(TEXT("%s %i %s"), *Separator, RefundItem.Quantity, *RefundItemData->DisplayName));
		InitialElement = false;
	}

	DestroyRefundTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Refund%s"), *RefundString)));
}

void UBuildingHammerWidget::SetupRotateText()
{
	if (Deployable == nullptr || RotatePanel == nullptr || RotateTextBlock == nullptr)
	{
		return;
	}

	if (!Deployable->CanRotate())
	{
		RotateTextBlock->SetText(FText::FromString(TEXT("Cannot Rotate")));
		RotateTextBlock->SetIsEnabled(false);
		return;
	}

	RotateTextBlock->SetText(FText::FromString(TEXT("Rotate")));
	RotateTextBlock->SetIsEnabled(true);
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
