// Copyright Telephone Studios. All Rights Reserved.


#include "OptionBoxes/KeyOptionBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UKeyOptionBox::NativeConstruct()
{
	AwaitingInput = false;
	Button->OnClicked.AddDynamic(this, &UKeyOptionBox::OnClicked);
}

FReply UKeyOptionBox::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (AwaitingInput == false)
	{
		return FReply::Handled();
	}

	UE_LOG(LogTemp, Warning, TEXT("KeyPressed: %s"), *InKeyEvent.GetKey().ToString());
	AwaitingInput = false;

	return FReply::Handled();
}

void UKeyOptionBox::OnClicked()
{
	if (AwaitingInput)
	{
		return;
	}
	AwaitingInput = true;

}
