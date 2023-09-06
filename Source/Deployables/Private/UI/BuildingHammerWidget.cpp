// Copyright Telephone Studios. All Rights Reserved.


#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Deployables/BuildingBlock.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Log.h"

void UBuildingHammerWidget::Setup(ADeployable* InDeployable)
{
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
		UpgradeTextBlock->SetText(FText::FromString(GetUpgradeString(BuildingBlock)));
	}
	else
	{
		UpgradeTextBlock->SetText(FText::FromString(TEXT("Not Upgradable")));
		UpgradeTextBlock->SetIsEnabled(false);
	}
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

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	float MousePositionX = 0;
	float MousePositionY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	PlayerController->GetMousePosition(MousePositionX, MousePositionY);
	
	const int32 ViewportCenterX = ViewportSizeX * 0.5f;
	const int32 ViewportCenterY = ViewportSizeY * 0.5f;
	
	UpgradeSelected = FMath::RoundToInt32(MousePositionX) < (ViewportCenterX - 100);
	ScaleWidgetByBool(UpgradePanel, UpgradeSelected);

	DestroySelected = FMath::RoundToInt32(MousePositionX) > (ViewportCenterX + 100);
	ScaleWidgetByBool(DestroyTextBlock, DestroySelected);

	if (ButtonSound &&
		(UpgradeSelected && !UpgradeAlreadySelected) || (DestroySelected && !DestroyAlreadySelected))
	{
		PlaySound(ButtonSound);
	}

}

FReply UBuildingHammerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	// TODO Do Desired Action
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

FString UBuildingHammerWidget::GetUpgradeString(ABuildingBlock* BuildingBlock) const
{
	if (BuildingBlock == nullptr)
	{
		return TEXT("");
	}

	FInventoryItem UpgradeCost;
	switch (BuildingBlock->GetUpgradeDefaultClass()->GetMaterialType())
	{
	case EToolType::STONE:
		UpgradeCost.Name = TEXT("stone");
		break;
	case EToolType::METAL:
		UpgradeCost.Name = TEXT("metal");
		break;
	}
	UpgradeCost.Quantity = 50;

	FItemData* ItemData = UInventoryComponent::GetItemData(UpgradeCost.Name);
	if (ItemData == nullptr)
	{
		UE_LOG(LogDeployables, Warning, TEXT("Item Data for '%s' wasn't availible."), *UpgradeCost.Name.ToString());
		return TEXT("");
	}

	FString UpgradeString = FString::Printf(TEXT("Upgrade To %s (%i %s)"), *ItemData->DisplayName, UpgradeCost.Quantity, *ItemData->DisplayName);
	return UpgradeString;
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
