// Copyright Telephone Studios. All Rights Reserved.


#include "InputActionRemapper.h"
#include "Components/TextBlock.h"
#include "OptionBoxes/KeyOptionBox.h"
#include "InputAction.h"

void UInputActionRemapper::SetInputAction(UInputAction* InInputAction)
{
	InputAction = InInputAction;
	if (InputAction == nullptr)
	{
		return;
	}

	ActionTextBlock->SetText(FText::FromString(InputAction->GetFName().ToString()));
}

UInputAction* UInputActionRemapper::GetInputAction() const
{
	return InputAction;
}

UKeyOptionBox* UInputActionRemapper::GetKeyOptionBox() const
{
	return KeyOptionBox;
}
