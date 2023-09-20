// Copyright Telephone Studios. All Rights Reserved.


#include "GameplaySettingsWidget.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "WildOmissionGameUserSettings.h"
#include "Interfaces/CharacterSettingsInterface.h"

void UGameplaySettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FieldOfViewSliderOptionBox->SetMinValue(60.0f);
	FieldOfViewSliderOptionBox->SetMaxValue(110.0f);
}

void UGameplaySettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetShowBranding(ShowBrandingCheckOptionBox->IsChecked());
	UserSettings->SetShowCrosshair(ShowBrandingCheckOptionBox->IsChecked());
	UserSettings->SetCameraShakeEnabled(CameraShakeEnabledCheckOptionBox->IsChecked());
	UserSettings->SetFieldOfView(FieldOfViewSliderOptionBox->GetValue());

	ICharacterSettingsInterface* CharacterSettingsInterface = GetOwningPlayerPawn<ICharacterSettingsInterface>();
	if (CharacterSettingsInterface)
	{
		CharacterSettingsInterface->ApplyGameplaySettings();
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

	ShowBrandingCheckOptionBox->SetChecked(UserSettings->GetShowBranding());
	ShowCrosshairCheckOptionBox->SetChecked(UserSettings->GetShowCrosshair());
	CameraShakeEnabledCheckOptionBox->SetChecked(UserSettings->GetCameraShakeEnabled());
	FieldOfViewSliderOptionBox->SetValue(FieldOfView);
}
