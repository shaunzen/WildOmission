// Copyright Telephone Studios. All Rights Reserved.


#include "WorldRowWidget.h"
#include "Components/Button.h"
#include "WorldSelectionWidget.h"

void UWorldRowWidget::Setup(UWorldSelectionWidget* InParent, const FString& InWorldName)
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