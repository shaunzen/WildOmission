// Copyright Telephone Studios. All Rights Reserved.


#include "GameplaySettingsWidget.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "WildOmissionGameUserSettings.h"
#include "Interfaces/GameSettingsInterface.h"
#include "Interfaces/CharacterSettingsInterface.h"

void UGameplaySettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FieldOfViewSliderOptionBox->SetMinValue(60.0f);
	FieldOfViewSliderOptionBox->SetMaxValue(110.0f);

	MasterVolumeSliderOptionBox->SetMinValue(0.0f);
	MasterVolumeSliderOptionBox->SetMaxValue(100.0f);
}

void UGameplaySettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetFieldOfView(FieldOfViewSliderOptionBox->GetValue());
	UserSettings->SetMasterVolume(MasterVolumeSliderOptionBox->GetValue() / 100.0f);

	IGameSettingsInterface* GameSettingsInterface = Cast<IGameSettingsInterface>(GetWorld()->GetGameInstance());
	if (GameSettingsInterface)
	{
		GameSettingsInterface->ApplyMasterVolume();
	}

	ICharacterSettingsInterface* CharacterSettingsInterface = GetOwningPlayerPawn<ICharacterSettingsInterface>();
	if (CharacterSettingsInterface)
	{
		CharacterSettingsInterface->ApplyFieldOfView();
	}
}

void UGameplaySettingsWidget::OnRefresh()
{
	Super::OnRefresh();
	
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	float FieldOfView = UserSettings->GetFieldOfView();
	float MasterVolume = UserSettings->GetMasterVolume() * 100.0f;

	FieldOfViewSliderOptionBox->SetValue(FieldOfView);
	MasterVolumeSliderOptionBox->SetValue(MasterVolume);
}
