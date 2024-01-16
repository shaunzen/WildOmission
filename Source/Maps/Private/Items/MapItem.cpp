// Copyright Telephone Studios. All Rights Reserved.


#include "Items/MapItem.h"
#include "UI/MapWidget.h"
#include "UObject/ConstructorHelpers.h"

AMapItem::AMapItem()
{
	MapWidgetClass = nullptr;
	OpenWidget = nullptr;
	
	static ConstructorHelpers::FClassFinder<UMapWidget> MapWidgetBlueprint(TEXT("/Game/Maps/UI/WBP_MapWidget"));
	if (MapWidgetBlueprint.Succeeded())
	{
		MapWidgetClass = MapWidgetBlueprint.Class;
	}
}

void AMapItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	BringUpMap();
}

void AMapItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	BringUpMap();
}

void AMapItem::BringUpMap()
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	if (OpenWidget || MapWidgetClass == nullptr)
	{
		return;
	}

	OpenWidget = CreateWidget<UMapWidget>(OwnerPlayerController, MapWidgetClass);

	OpenWidget->AddToViewport();
}