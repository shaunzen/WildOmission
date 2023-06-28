// Copyright Telephone Studios. All Rights Reserved.


#include "UI/FurnaceWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/InventoryComponent.h"
#include "Deployables/Furnace.h"

void UFurnaceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ToggleButton == nullptr)
	{
		return;
	}

	ToggleButton->OnClicked.AddDynamic(this, &UFurnaceWidget::ToggleButtonPressed);
}

void UFurnaceWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AFurnace* ThisFurnace = Cast<AFurnace>(GetInventoryComponent()->GetOwner());
	if (ThisFurnace == nullptr)
	{
		return;
	}

	FString ToggleString = FString();
	if (ThisFurnace->IsTurnedOn())
	{
		ToggleString = FString("Turn Off");
	}
	else
	{
		ToggleString = FString("Turn On");
	}

	ToggleText->SetText(FText::FromString(ToggleString));
}

void UFurnaceWidget::ToggleButtonPressed()
{
	// get furnace we are in
	if (GetInventoryComponent() == nullptr)
	{
		return;
	}

	AFurnace* ThisFurnace = Cast<AFurnace>(GetInventoryComponent()->GetOwner());
	if (ThisFurnace == nullptr)
	{
		return;
	}

	// toggle its on state
	ThisFurnace->Server_ToggleState(!ThisFurnace->IsTurnedOn());
}