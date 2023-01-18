// Copyright Telephone Studios. All Rights Reserved.


#include "SaveRowWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.h"

void USaveRowWidget::Setup(UMainMenuWidget* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &USaveRowWidget::OnClicked);
}

void USaveRowWidget::OnClicked()
{
	Parent->SelectSaveIndex(Index);
}