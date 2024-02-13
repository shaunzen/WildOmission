// Copyright Telephone Studios. All Rights Reserved.


#include "UI/SignWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/BuilderComponent.h"
#include "Deployables/Sign.h"

USignWidget::USignWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);

	LineOneInputBox = nullptr;
	LineTwoInputBox = nullptr;
	LineThreeInputBox = nullptr;
	LineFourInputBox = nullptr;

	DoneButton = nullptr;
	CancelButton = nullptr;
	Sign = nullptr;
}

void USignWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	LineOneInputBox->OnTextChanged.AddDynamic(this, &USignWidget::OnLineOneInputBoxTextChanged);
	LineTwoInputBox->OnTextChanged.AddDynamic(this, &USignWidget::OnLineTwoInputBoxTextChanged);
	LineThreeInputBox->OnTextChanged.AddDynamic(this, &USignWidget::OnLineThreeInputBoxTextChanged);
	LineFourInputBox->OnTextChanged.AddDynamic(this, &USignWidget::OnLineFourInputBoxTextChanged);

	DoneButton->OnClicked.AddDynamic(this, &USignWidget::OnDoneButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &USignWidget::OnCancelButtonClicked);
}

void USignWidget::Setup(ASign* InSign)
{
	Sign = InSign;

	if (Sign == nullptr)
	{
		this->Teardown();
		return;
	}

	const TArray<FString> SignText = Sign->GetText();
	if (SignText.IsValidIndex(0))
	{
		LineOneInputBox->SetText(FText::FromString(SignText[0]));
	}
	if (SignText.IsValidIndex(1))
	{
		LineTwoInputBox->SetText(FText::FromString(SignText[1]));
	}
	if (SignText.IsValidIndex(2))
	{
		LineThreeInputBox->SetText(FText::FromString(SignText[2]));
	}
	if (SignText.IsValidIndex(3))
	{
		LineFourInputBox->SetText(FText::FromString(SignText[3]));
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	this->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
}

void USignWidget::Teardown()
{
	Sign = nullptr;
	
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

FReply USignWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		this->Teardown();
	}

	return FReply::Handled();
}

void USignWidget::OnLineOneInputBoxTextChanged(const FText& Text)
{
	PerformLineTrimOnInputBox(LineOneInputBox, Text);
}

void USignWidget::OnLineTwoInputBoxTextChanged(const FText& Text)
{
	PerformLineTrimOnInputBox(LineTwoInputBox, Text);
}

void USignWidget::OnLineThreeInputBoxTextChanged(const FText& Text)
{
	PerformLineTrimOnInputBox(LineThreeInputBox, Text);
}

void USignWidget::OnLineFourInputBoxTextChanged(const FText& Text)
{
	PerformLineTrimOnInputBox(LineFourInputBox, Text);
}

void USignWidget::PerformLineTrimOnInputBox(UEditableTextBox* TextBox, const FText& Text)
{
	FString TextString = Text.ToString();
	
	if (TextString.Len() > 22)
	{
		TextString = TextString.LeftChop(1);
	}

	TextBox->SetText(FText::FromString(TextString));
}

void USignWidget::OnDoneButtonClicked()
{
	APawn* OwnerPawn = GetOwningPlayerPawn();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UBuilderComponent* BuilderComponent = OwnerPawn->FindComponentByClass<UBuilderComponent>();
	if (BuilderComponent == nullptr)
	{
		return;
	}

	TArray<FString> SignString = { 
		LineOneInputBox->GetText().ToString(),
		LineTwoInputBox->GetText().ToString(),
		LineThreeInputBox->GetText().ToString(),
		LineFourInputBox->GetText().ToString() };

	/*const FString SignString = LineOneInputBox->GetText().ToString() + LINE_TERMINATOR + LineTwoInputBox->GetText().ToString()
		+ LINE_TERMINATOR + LineThreeInputBox->GetText().ToString() + LINE_TERMINATOR + LineFourInputBox->GetText().ToString();*/

	BuilderComponent->Server_ChangeSignText(Sign, SignString);

	this->Teardown();
}

void USignWidget::OnCancelButtonClicked()
{
	this->Teardown();
}