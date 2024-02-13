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
		TextString = TextString.RightChop(1);
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

	
	//BuilderComponent->Server_ChangeSignText(Sign, TextBox->GetText().ToString());
}

void USignWidget::OnCancelButtonClicked()
{
	this->Teardown();
}