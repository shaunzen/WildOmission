// Copyright Telephone Studios. All Rights Reserved.


#include "UI/DeleteWorldWidget.h"
#include "Components/Button.h"

UDeleteWorldWidget::UDeleteWorldWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	DeleteButton = nullptr;
	CancelButton = nullptr;
}

void UDeleteWorldWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DeleteButton->OnClicked.AddDynamic(this, &UDeleteWorldWidget::BroadcastDeleteButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UDeleteWorldWidget::BroadcastCancelButtonClicked);
}

void UDeleteWorldWidget::BroadcastDeleteButtonClicked()
{
	if (!OnDeleteButtonClicked.IsBound())
	{
		return;
	}

	OnDeleteButtonClicked.Broadcast();
}

void UDeleteWorldWidget::BroadcastCancelButtonClicked()
{
	if (!OnCancelButtonClicked.IsBound())
	{
		return;
	}

	OnCancelButtonClicked.Broadcast();
}
