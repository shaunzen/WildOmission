// Copyright Telephone Studios. All Rights Reserved.


#include "PostProcessingSettingsWidget.h"
#include "WildOmissionGameUserSettings.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "Interfaces/CharacterSettingsInterface.h"

UPostProcessingSettingsWidget::UPostProcessingSettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	GammaSliderOptionBox = nullptr;
	AutoExposureCheckOptionBox = nullptr;
	MotionBlurCheckOptionBox = nullptr;
	BloomCheckOptionBox = nullptr;
	AmbientOcclusionCheckOptionBox = nullptr;
	FilmGrainCheckOptionBox = nullptr;
}

void UPostProcessingSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GammaSliderOptionBox->SetMinValue(80.0f);
	GammaSliderOptionBox->SetMaxValue(120.0f);
	AutoExposureCheckOptionBox->SetChecked(true);
	MotionBlurCheckOptionBox->SetChecked(false);
	BloomCheckOptionBox->SetChecked(true);
	AmbientOcclusionCheckOptionBox->SetChecked(true);
	FilmGrainCheckOptionBox->SetChecked(false);

	GammaSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UPostProcessingSettingsWidget::OnApply);
	AutoExposureCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UPostProcessingSettingsWidget::OnApply);
	MotionBlurCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UPostProcessingSettingsWidget::OnApply);
	BloomCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UPostProcessingSettingsWidget::OnApply);
	AmbientOcclusionCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UPostProcessingSettingsWidget::OnApply);
	FilmGrainCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UPostProcessingSettingsWidget::OnApply);
}

void UPostProcessingSettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}
	
	UserSettings->SetGamma(GammaSliderOptionBox->GetValue());
	UserSettings->SetAutoExposureEnabled(AutoExposureCheckOptionBox->IsChecked());
	UserSettings->SetMotionBlurEnabled(MotionBlurCheckOptionBox->IsChecked());
	UserSettings->SetBloomEnabled(BloomCheckOptionBox->IsChecked());
	UserSettings->SetAmbientOcclusionEnabled(AmbientOcclusionCheckOptionBox->IsChecked());
	UserSettings->SetFilmGrainEnabled(FilmGrainCheckOptionBox->IsChecked());

	ICharacterSettingsInterface* CharacterSettingsInterface = GetOwningPlayerPawn<ICharacterSettingsInterface>();
	if (CharacterSettingsInterface == nullptr)
	{
		return;
	}
	
	CharacterSettingsInterface->ApplyPostProcessingSettings();

	UserSettings->ApplySettings(false);

	OnRefresh();
}

void UPostProcessingSettingsWidget::OnRefresh()
{
	Super::OnRefresh();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	GammaSliderOptionBox->SetValue(UserSettings->GetGamma());
	AutoExposureCheckOptionBox->SetChecked(UserSettings->GetAutoExposureEnabled());
	MotionBlurCheckOptionBox->SetChecked(UserSettings->GetMotionBlurEnabled());
	BloomCheckOptionBox->SetChecked(UserSettings->GetBloomEnabled());
	AmbientOcclusionCheckOptionBox->SetChecked(UserSettings->GetAmbientOcclusionEnabled());
	FilmGrainCheckOptionBox->SetChecked(UserSettings->GetFilmGrainEnabled());
}
