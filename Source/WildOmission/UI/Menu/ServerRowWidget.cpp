// Copyright Telephone Studios. All Rights Reserved.


#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "ServerBrowserWidget.h"

void UServerRowWidget::Setup(UServerBrowserWidget* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnClicked);
}

void UServerRowWidget::OnClicked()
{
	Parent->SelectServerIndex(Index);
}