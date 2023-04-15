// Copyright Telephone Studios. All Rights Reserved.


#include "WorldRowWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.h"

void UWorldRowWidget::Setup(UMainMenuWidget* InParent, const FString& InWorldName)
{
	Parent = InParent;
	WorldName = InWorldName;

	RowButton->OnClicked.AddDynamic(this, &UWorldRowWidget::OnClicked);
}

FString UWorldRowWidget::GetWorldName()
{
	return WorldName;
}

void UWorldRowWidget::OnClicked()
{
	Parent->SetSelectedWorld(WorldName);
}