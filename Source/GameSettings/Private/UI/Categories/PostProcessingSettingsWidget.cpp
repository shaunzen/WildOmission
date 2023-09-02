// Copyright Telephone Studios. All Rights Reserved.


#include "PostProcessingSettingsWidget.h"
#include "WildOmissionGameUserSettings.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "Interfaces/CharacterSettingsInterface.h"

void UPostProcessingSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AutoExposureCheckOptionBox->SetChecked(true);
	MotionBlurCheckOptionBox->SetChecked(false);
	BloomCheckOptionBox->SetChecked(true);
	AmbientOcclusionCheckOptionBox->SetChecked(true);
	FilmGrainCheckOptionBox->SetChecked(false);
}

void UPostProcessingSettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}
	
	UserSettings->SetAutoExposureEnabled(AutoExposureCheckOptionBox->IsChecked());
	UserSettings->SetMotionBlurEnabled(MotionBlurCheckOptionBox->IsChecked());
	UserSettings->SetBloomEnabled(BloomCheckOptionBox->IsChecked());
	UserSettings->SetAmbientOcclusionEnabled(AmbientOcclusionCheckOptionBox->IsChecked());
	UserSettings->SetFilmGrainEnabled(FilmGrainCheckOptionBox->IsChecked());

	ICharacterSettingsInterface* CharacterSettingsInterface = GetOwningPlayerPawn<ICharacterSettingsInterface>();
	if (CharacterSettingsInterface)
	{
		CharacterSettingsInterface->ApplyPostProcessing();
	}
}

void UPostProcessingSettingsWidget::OnRefresh()
{
	Super::OnRefresh();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	AutoExposureCheckOptionBox->SetChecked(UserSettings->GetAutoExposureEnabled());
	MotionBlurCheckOptionBox->SetChecked(UserSettings->GetMotionBlurEnabled());
	BloomCheckOptionBox->SetChecked(UserSettings->GetBloomEnabled());
	AmbientOcclusionCheckOptionBox->SetChecked(UserSettings->GetAmbientOcclusionEnabled());
	FilmGrainCheckOptionBox->SetChecked(UserSettings->GetFilmGrainEnabled());
}
