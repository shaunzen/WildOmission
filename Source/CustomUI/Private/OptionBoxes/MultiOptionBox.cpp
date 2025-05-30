// Copyright Telephone Studios. All Rights Reserved.


#include "OptionBoxes/MultiOptionBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UMultiOptionBox::UMultiOptionBox(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	NextButton = nullptr;
	PreviousButton = nullptr;
	SelectedTextBlock = nullptr;

	SelectedOptionIndex = 0;
}

void UMultiOptionBox::NativeConstruct()
{
	Super::NativeConstruct();

	NextButton->OnClicked.AddDynamic(this, &UMultiOptionBox::NextOption);
	PreviousButton->OnClicked.AddDynamic(this, &UMultiOptionBox::PreviousOption);
}

void UMultiOptionBox::AddOption(const FString& NewOption)
{
	Options.Add(NewOption);
}

void UMultiOptionBox::GiveQualityOptions()
{
	ClearOptions();
	AddOption(TEXT("Low"));
	AddOption(TEXT("Medium"));
	AddOption(TEXT("High"));
	AddOption(TEXT("Epic"));
	AddOption(TEXT("Cinematic"));
}

void UMultiOptionBox::SetSelectedOption(const FString& NewOption)
{
	SelectedOptionIndex = FMath::Clamp(Options.Find(NewOption), 0, Options.Num() - 1);
	Refresh();
}

void UMultiOptionBox::SetSelectedIndex(const int32& NewIndex)
{
	SelectedOptionIndex = FMath::Clamp(NewIndex, 0, Options.Num() - 1);
	Refresh();
}

void UMultiOptionBox::ClearOptions()
{
	Options.Empty();
}

FString UMultiOptionBox::GetSelectedOption() const
{
	if (!Options.IsValidIndex(SelectedOptionIndex))
	{
		return FString();
	}

	return Options[SelectedOptionIndex];
}

int32 UMultiOptionBox::GetSelectedIndex() const
{
	return SelectedOptionIndex;
}

void UMultiOptionBox::NextOption()
{
	SelectedOptionIndex++;
	if (SelectedOptionIndex > Options.Num() - 1)
	{
		SelectedOptionIndex = 0;
	}

	Refresh();
}

void UMultiOptionBox::PreviousOption()
{
	SelectedOptionIndex--;
	if (SelectedOptionIndex < 0)
	{
		SelectedOptionIndex = Options.Num() - 1;
	}

	Refresh();
}

void UMultiOptionBox::Refresh()
{
	if (!Options.IsValidIndex(SelectedOptionIndex))
	{
		return;
	}

	SelectedTextBlock->SetText(FText::FromString(Options[SelectedOptionIndex]));

	if (OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(Options[SelectedOptionIndex]);
	}

	if (OnValueChangedNoParams.IsBound())
	{
		OnValueChangedNoParams.Broadcast();
	}
}
