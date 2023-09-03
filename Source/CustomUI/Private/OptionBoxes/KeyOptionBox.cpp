// Copyright Telephone Studios. All Rights Reserved.


#include "OptionBoxes/KeyOptionBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UKeyOptionBox::NativeConstruct()
{
	AwaitingInput = false;
	SelectedKey = EKeys::Semicolon;
	Button->OnClicked.AddDynamic(this, &UKeyOptionBox::OnClicked);
	RefreshTextBlock();
}

void UKeyOptionBox::SetSelectedKey(const FKey& NewSelectedKey)
{
	SelectedKey = NewSelectedKey;
	RefreshTextBlock();
}

FKey UKeyOptionBox::GetSelectedKey() const
{
	return SelectedKey;
}

FReply UKeyOptionBox::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (AwaitingInput == false)
	{
		return FReply::Handled();
	}
	
	AwaitingInput = false;
	SetSelectedKey(InKeyEvent.GetKey());
	
	return FReply::Handled();
}

void UKeyOptionBox::RefreshTextBlock()
{
	FString DisplayString = SelectedKey.ToString();

	if (AwaitingInput)
	{
		DisplayString = TEXT("Press Desired Key.");
	}

	TextBlock->SetText(FText::FromString(DisplayString));
}

void UKeyOptionBox::OnClicked()
{
	if (AwaitingInput)
	{
		return;
	}
	
	AwaitingInput = true;
	RefreshTextBlock();
}
