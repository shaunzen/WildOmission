// Copyright Telephone Studios. All Rights Reserved.


#include "UI/ConnectedPlayersWidget.h"
#include "Components/Button.h"

UConnectedPlayersWidget::UConnectedPlayersWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	CloseButton = nullptr;
	ConnectedPlayersPanel = nullptr;
}

void UConnectedPlayersWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UConnectedPlayersWidget::Back);
	}
}

void UConnectedPlayersWidget::Refresh()
{
	// TODO
}

void UConnectedPlayersWidget::Back()
{
	if (!OnBackButtonClicked.IsBound())
	{
		return;
	}

	OnBackButtonClicked.Broadcast();
}
