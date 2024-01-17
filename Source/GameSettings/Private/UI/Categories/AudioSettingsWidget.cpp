// Copyright Telephone Studios. All Rights Reserved.


#include "AudioSettingsWidget.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "WildOmissionGameUserSettings.h"
#include "Interfaces/GameSettingsInterface.h"

UAudioSettingsWidget::UAudioSettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	MasterVolumeSliderOptionBox = nullptr;
	MusicVolumeSliderOptionBox = nullptr;
	DeployablesVolumeSliderOptionBox = nullptr;
	EnvironmentVolumeSliderOptionBox = nullptr;
	FriendlyCreaturesVolumeSliderOptionBox = nullptr;
	HostileCreaturesVolumeSliderOptionBox = nullptr;
	PlayersVolumeSliderOptionBox = nullptr;
	WeatherVolumeSliderOptionBox = nullptr;
}

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

	MasterVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	MusicVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	DeployablesVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	EnvironmentVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	FriendlyCreaturesVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	HostileCreaturesVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	PlayersVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
	WeatherVolumeSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UAudioSettingsWidget::OnApply);
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

	IGameSettingsInterface* GameSettingsInterface = Cast<IGameSettingsInterface>(GetWorld()->GetGameInstance());
	if (GameSettingsInterface == nullptr)
	{
		return;
	}

	GameSettingsInterface->ApplyAudioSettings();
	
	UserSettings->ApplySettings(false);

	OnRefresh();
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
