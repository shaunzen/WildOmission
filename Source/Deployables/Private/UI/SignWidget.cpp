// Copyright Telephone Studios. All Rights Reserved.


#include "UI/SignWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Components/BuilderComponent.h"
#include "Deployables/Sign.h"

USignWidget::USignWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);

	TextBox = nullptr;
	DoneButton = nullptr;
	CancelButton = nullptr;
	Sign = nullptr;
}

void USignWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox->OnTextChanged.AddDynamic(this, &USignWidget::OnTextBoxTextChanged);
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

void USignWidget::OnTextBoxTextChanged(const FText& Text)
{
	// TODO figure this shit out
	TextBox->OnTextChanged.RemoveDynamic(this, &USignWidget::OnTextBoxTextChanged);
	
	const FString TextString = Text.ToString();

	FString FormatedString = TEXT("");
	for (int32 i = 0; i < TextString.Len(); ++i)
	{
		FormatedString.AppendChar(TextString[i]);
		const FString CurrentLookAhead = FString::Printf(TEXT("%s%s%s%s"), TextString[i], TextString[i + 1], TextString[i + 2], TextString[i + 3]);
		if (i % 22 == 0 && CurrentLookAhead != LINE_TERMINATOR)
		{
			FormatedString.Append(LINE_TERMINATOR);
		}
	}

	TextBox->SetText(FText::FromString(FormatedString));

	TextBox->OnTextChanged.AddDynamic(this, &USignWidget::OnTextBoxTextChanged);
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

	BuilderComponent->Server_ChangeSignText(Sign, TextBox->GetText().ToString());
}

void USignWidget::OnCancelButtonClicked()
{
	this->Teardown();
}