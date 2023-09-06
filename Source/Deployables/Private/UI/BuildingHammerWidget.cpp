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

FReply UBuildingHammerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return FReply::Handled();
	}

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	const int32 ViewportCenterX = ViewportSizeX * 0.5f;
	const int32 ViewportCenterY = ViewportSizeY * 0.5f;
	const FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();

	if (MousePosition.X < (ViewportCenterX - 100))
	{
		// Upgrade
		UE_LOG(LogTemp, Warning, TEXT("Upgrading Building Block."));
	}
	else if (MousePosition.X > (ViewportCenterX + 100))
	{
		// Destroy
		UE_LOG(LogTemp, Warning, TEXT("Destroying Deployable."));
	}

	
	this->Teardown();

	return FReply::Handled();
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
