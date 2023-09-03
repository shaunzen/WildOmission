// Copyright Telephone Studios. All Rights Reserved.


#include "InputActionRemapper.h"
#include "Components/TextBlock.h"
#include "OptionBoxes/KeyOptionBox.h"

void UInputActionRemapper::SetActionName(const FString& ActionName)
{
	ActionTextBlock->SetText(FText::FromString(ActionName));
}

UKeyOptionBox* UInputActionRemapper::GetKeyOptionBox() const
{
	return KeyOptionBox;
}
