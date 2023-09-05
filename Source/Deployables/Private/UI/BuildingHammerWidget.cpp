// Copyright Telephone Studios. All Rights Reserved.


#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Deployables/BuildingBlock.h"
#include "Components/TextBlock.h"

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

	// TODO figure out which side we were leaning towards
	this->Teardown();

	return FReply::Handled();
}

FString UBuildingHammerWidget::GetUpgradeString(ABuildingBlock* BuildingBlock) const
{
	FString UpgradeString = FString::Printf(TEXT("Upgrade To %s (%i %s)"), /*buildtier*/, /*upgrade resource quantity*/, /*upgrade resource name*/);
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
