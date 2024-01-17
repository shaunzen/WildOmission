// Copyright Telephone Studios. All Rights Reserved.


#include "GameplaySettingsWidget.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "WildOmissionGameUserSettings.h"
#include "Interfaces/CharacterSettingsInterface.h"

UGameplaySettingsWidget::UGameplaySettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	ShowBrandingCheckOptionBox = nullptr;
	ShowCrosshairCheckOptionBox = nullptr;
	HideChatUnlessOpenCheckOptionBox = nullptr;
	HideHUDCheckOptionBox = nullptr;
	CameraShakeEnabledCheckOptionBox = nullptr;
	FieldOfViewSliderOptionBox = nullptr;
}

void UGameplaySettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FieldOfViewSliderOptionBox->SetMinValue(60.0f);
	FieldOfViewSliderOptionBox->SetMaxValue(110.0f);

	ShowBrandingCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	ShowCrosshairCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	HideChatUnlessOpenCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	HideHUDCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	CameraShakeEnabledCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	FieldOfViewSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
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
	UserSettings->SetShowCrosshair(ShowCrosshairCheckOptionBox->IsChecked());
	UserSettings->SetHideChatUnlessOpen(HideChatUnlessOpenCheckOptionBox->IsChecked());
	UserSettings->SetHideHUD(HideHUDCheckOptionBox->IsChecked());
	UserSettings->SetCameraShakeEnabled(CameraShakeEnabledCheckOptionBox->IsChecked());
	UserSettings->SetFieldOfView(FieldOfViewSliderOptionBox->GetValue());

	ICharacterSettingsInterface* CharacterSettingsInterface = GetOwningPlayerPawn<ICharacterSettingsInterface>();
	if (CharacterSettingsInterface == nullptr)
	{
		return;
	}
	
	CharacterSettingsInterface->ApplyGameplaySettings();

	UserSettings->ApplySettings(false);
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
	HideChatUnlessOpenCheckOptionBox->SetChecked(UserSettings->GetHideChatUnlessOpen());
	HideHUDCheckOptionBox->SetChecked(UserSettings->GetHideHUD());
	CameraShakeEnabledCheckOptionBox->SetChecked(UserSettings->GetCameraShakeEnabled());
	FieldOfViewSliderOptionBox->SetValue(FieldOfView);
}
