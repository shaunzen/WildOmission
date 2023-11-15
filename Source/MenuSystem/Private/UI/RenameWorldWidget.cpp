// Copyright Telephone Studios. All Rights Reserved.


#include "UI/RenameWorldWidget.h"
#include "Components/Button.h"

URenameWorldWidget::URenameWorldWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	RenameButton = nullptr;
	CancelButton = nullptr;
}

void URenameWorldWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RenameButton->OnClicked.AddDynamic(this, &URenameWorldWidget::BroadcastRenameButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &URenameWorldWidget::BroadcastCancelButtonClicked);
}

void URenameWorldWidget::BroadcastRenameButtonClicked()
{
	if (!OnRenameButtonClicked.IsBound())
	{
		return;
	}

	OnRenameButtonClicked.Broadcast();
}

void URenameWorldWidget::BroadcastCancelButtonClicked()
{
	if (!OnCancelButtonClicked.IsBound())
	{
		return;
	}

	OnCancelButtonClicked.Broadcast();
}
