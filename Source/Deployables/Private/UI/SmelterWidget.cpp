// Copyright Telephone Studios. All Rights Reserved.


#include "UI/SmelterWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/InventoryComponent.h"
#include "Deployables/ItemSmelterBase.h"

void USmelterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ToggleButton == nullptr)
	{
		return;
	}

	ToggleButton->OnClicked.AddDynamic(this, &USmelterWidget::ToggleButtonPressed);
}

void USmelterWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	AItemSmelterBase* ThisSmelter = Cast<AItemSmelterBase>(GetInventoryComponent()->GetOwner());
	if (ThisSmelter == nullptr)
	{
		return;
	}

	FString ToggleString = FString();
	if (ThisSmelter->IsTurnedOn())
	{
		ToggleString = FString("Turn Off");
	}
	else
	{
		ToggleString = FString("Turn On");
	}

	ToggleText->SetText(FText::FromString(ToggleString));
}

void USmelterWidget::ToggleButtonPressed()
{
	// get furnace we are in
	if (GetInventoryComponent() == nullptr)
	{
		return;
	}

	AItemSmelterBase* ThisSmelter = Cast<AItemSmelterBase>(GetInventoryComponent()->GetOwner());
	if (ThisSmelter == nullptr)
	{
		return;
	}

	// toggle its on state
	ThisSmelter->Server_ToggleState(!ThisSmelter->IsTurnedOn());
}