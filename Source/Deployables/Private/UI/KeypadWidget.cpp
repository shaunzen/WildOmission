// Copyright Telephone Studios. All Rights Reserved.


#include "UI/KeypadWidget.h"

UKeypadWidget::UKeypadWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	CodeTextBlock = nullptr;
	OneKeyButton = nullptr;
	TwoKeyButton = nullptr;
	ThreeKeyButton = nullptr;
	FourKeyButton = nullptr;
	FiveKeyButton = nullptr;
	SixKeyButton = nullptr;
	SevenKeyButton = nullptr;
	EightKeyButton = nullptr;
	NineKeyButton = nullptr;
	ZeroKeyButton = nullptr;
	ToggleLockButton = nullptr;
	ToggleLockTextBlock = nullptr;
	CloseButton = nullptr;
}
