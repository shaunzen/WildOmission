// Copyright Telephone Studios. All Rights Reserved.


#include "UI/RenameWorldWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

URenameWorldWidget::URenameWorldWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	NameInputBox = nullptr;
	RenameButton = nullptr;
	CancelButton = nullptr;
}

void URenameWorldWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RenameButton->OnClicked.AddDynamic(this, &URenameWorldWidget::RenameWorld);
	CancelButton->OnClicked.AddDynamic(this, &URenameWorldWidget::BroadcastCancelButtonClicked);
}

void URenameWorldWidget::RenameWorld()
{
	const FString NewWorldName = NameInputBox->GetText().ToString();
	BroadcastRenameButtonClicked(NewWorldName);
}

void URenameWorldWidget::BroadcastRenameButtonClicked(const FString& NewWorldName)
{
	if (!OnRenameButtonClicked.IsBound())
	{
		return;
	}

	OnRenameButtonClicked.Broadcast(NewWorldName);
}

void URenameWorldWidget::BroadcastCancelButtonClicked()
{
	if (!OnCancelButtonClicked.IsBound())
	{
		return;
	}

	OnCancelButtonClicked.Broadcast();
}
