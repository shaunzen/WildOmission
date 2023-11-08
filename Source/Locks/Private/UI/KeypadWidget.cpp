// Copyright Telephone Studios. All Rights Reserved.


#include "UI/KeypadWidget.h"
#include "Locks/Lock.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UKeypadWidget::UKeypadWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);

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
	LockActionButton = nullptr;
	LockActionTextBlock = nullptr;
	CloseButton = nullptr;

	PendingCode = TEXT("");

	LockToModify = nullptr;
}

void UKeypadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OneKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnOnePressed);
	TwoKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnTwoPressed);
	ThreeKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnThreePressed);
	FourKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnFourPressed);
	FiveKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnFivePressed);
	SixKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnSixPressed);
	SevenKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnSevenPressed);
	EightKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnEightPressed);
	NineKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnNinePressed);
	ZeroKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnZeroPressed);
	CloseButton->OnClicked.AddDynamic(this, &UKeypadWidget::Teardown);

	RefreshCodeTextBlock();
}

void UKeypadWidget::Setup(ALock* InLock)
{
	LockToModify = InLock;

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);

	this->AddToViewport();
}

void UKeypadWidget::Teardown()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(false);

	if (OnTeardown.IsBound())
	{
		OnTeardown.Broadcast();
	}

	this->RemoveFromParent();
}

void UKeypadWidget::OnOnePressed()
{
	AddCharacterToCode(TEXT("1"));
}

void UKeypadWidget::OnTwoPressed()
{
	AddCharacterToCode(TEXT("2"));
}

void UKeypadWidget::OnThreePressed()
{
	AddCharacterToCode(TEXT("3"));
}

void UKeypadWidget::OnFourPressed()
{
	AddCharacterToCode(TEXT("4"));
}

void UKeypadWidget::OnFivePressed()
{
	AddCharacterToCode(TEXT("5"));
}

void UKeypadWidget::OnSixPressed()
{
	AddCharacterToCode(TEXT("6"));
}

void UKeypadWidget::OnSevenPressed()
{
	AddCharacterToCode(TEXT("7"));
}

void UKeypadWidget::OnEightPressed()
{
	AddCharacterToCode(TEXT("8"));
}

void UKeypadWidget::OnNinePressed()
{
	AddCharacterToCode(TEXT("9"));
}

void UKeypadWidget::OnZeroPressed()
{
	AddCharacterToCode(TEXT("0"));
}

void UKeypadWidget::AddCharacterToCode(const FString& CharacterToAdd)
{
	PendingCode.Append(CharacterToAdd);
	if (PendingCode.Len() > 4)
	{
		PendingCode = PendingCode.RightChop(1);
	}

	RefreshCodeTextBlock();
}

void UKeypadWidget::RefreshCodeTextBlock()
{
	FString DisplayCode = TEXT("");
	for (int32 i = 0; i < PendingCode.Len(); i++)
	{
		DisplayCode.Append(TEXT("*"));
	}

	CodeTextBlock->SetText(FText::FromString(DisplayCode));
}
