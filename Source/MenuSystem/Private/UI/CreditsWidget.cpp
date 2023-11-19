// Copyright Telephone Studios. All Rights Reserved.


#include "UI/CreditsWidget.h"
#include "Components/Button.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	BackButton = nullptr;
}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UCreditsWidget::BroadcastOnBackButtonClicked);
	}
}

void UCreditsWidget::BroadcastOnBackButtonClicked()
{
	if (!OnBackButtonClicked.IsBound())
	{
		return;
	}

	OnBackButtonClicked.Broadcast();
}