// Copyright Telephone Studios. All Rights Reserved.


#include "AudioSettingsWidget.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "WildOmissionGameUserSettings.h"

void UAudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MasterVolumeSliderOptionBox->SetMinValue(0.0f);
	MasterVolumeSliderOptionBox->SetMaxValue(100.0f);
	MusicVolumeSliderOptionBox->SetMinValue(0.0f);
	MusicVolumeSliderOptionBox->SetMaxValue(100.0f);
	DeployablesVolumeSliderOptionBox->SetMinValue(0.0f);
	DeployablesVolumeSliderOptionBox->SetMaxValue(100.0f);
	EnvironmentVolumeSliderOptionBox->SetMinValue(0.0f);
	EnvironmentVolumeSliderOptionBox->SetMaxValue(100.0f);
	FriendlyCreaturesVolumeSliderOptionBox->SetMinValue(0.0f);
	FriendlyCreaturesVolumeSliderOptionBox->SetMaxValue(100.0f);
	HostileCreaturesVolumeSliderOptionBox->SetMinValue(0.0f);
	HostileCreaturesVolumeSliderOptionBox->SetMaxValue(100.0f);
	PlayersVolumeSliderOptionBox->SetMinValue(0.0f);
	PlayersVolumeSliderOptionBox->SetMaxValue(100.0f);
	WeatherVolumeSliderOptionBox->SetMinValue(0.0f);
	WeatherVolumeSliderOptionBox->SetMaxValue(100.0f);
}

void UAudioSettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetMasterVolume(MasterVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetMusicVolume(MusicVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetDeployablesVolume(DeployablesVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetEnvironmentVolume(EnvironmentVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetFriendlyCreaturesVolume(FriendlyCreaturesVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetHostileCreaturesVolume(HostileCreaturesVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetPlayersVolume(PlayersVolumeSliderOptionBox->GetValue() / 100.0f);
	UserSettings->SetWeatherVolume(WeatherVolumeSliderOptionBox->GetValue() / 100.0f);

	// TODO ask game instance to apply
}

void UAudioSettingsWidget::OnRefresh()
{
	Super::OnRefresh();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	MasterVolumeSliderOptionBox->SetValue(UserSettings->GetMasterVolume() * 100.0f);
	MusicVolumeSliderOptionBox->SetValue(UserSettings->GetMusicVolume() * 100.0f);
	DeployablesVolumeSliderOptionBox->SetValue(UserSettings->GetDeployablesVolume() * 100.0f);
	EnvironmentVolumeSliderOptionBox->SetValue(UserSettings->GetEnvironmentVolume() * 100.0f);
	FriendlyCreaturesVolumeSliderOptionBox->SetValue(UserSettings->GetFriendlyCreaturesVolume() * 100.0f);
	HostileCreaturesVolumeSliderOptionBox->SetValue(UserSettings->GetHostileCreaturesVolume() * 100.0f);
	PlayersVolumeSliderOptionBox->SetValue(UserSettings->GetPlayersVolume() * 100.0f);
	WeatherVolumeSliderOptionBox->SetValue(UserSettings->GetWeatherVolume() * 100.0f);
}
