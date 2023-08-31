// Copyright Telephone Studios. All Rights Reserved.


#include "UI/OptionsWidget.h"
#include "WildOmissionGameUserSettings.h"
#include "Interfaces/CharacterSettingsInterface.h"
#include "Interfaces/GameSettingsInterface.h"
#include "Components/Button.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "OptionBoxes/MultiOptionBox.h"
#include "Kismet/KismetSystemLibrary.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ApplyButton->OnClicked.AddDynamic(this, &UOptionsWidget::Apply);
	ResetButton->OnClicked.AddDynamic(this, &UOptionsWidget::Reset);
	BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::Back);
}

void UOptionsWidget::Refresh()
{
	// TODO refresh all children of widget switcher
}

void UOptionsWidget::Apply()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}
	// TODO apply settings of all children of widget switcher

	UserSettings->ApplySettings(false);
	Refresh();
}


void UOptionsWidget::Reset()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetToDefaults();
	Refresh();
}

void UOptionsWidget::Back()
{
	if (!OnBackButtonPressed.IsBound())
	{
		return;
	}

	OnBackButtonPressed.Broadcast();
}