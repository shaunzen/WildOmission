// Copyright Telephone Studios. All Rights Reserved.


#include "GameplaySettingsWidget.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "WildOmissionGameUserSettings.h"
#include "Interfaces/CharacterSettingsInterface.h"

UGameplaySettingsWidget::UGameplaySettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	FieldOfViewSliderOptionBox = nullptr;
	RenderDistanceSliderOptionBox = nullptr;
	ShowBrandingCheckOptionBox = nullptr;
	ShowCrosshairCheckOptionBox = nullptr;
	HideChatUnlessOpenCheckOptionBox = nullptr;
	HideHUDCheckOptionBox = nullptr;
	CameraShakeEnabledCheckOptionBox = nullptr;
}

void UGameplaySettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FieldOfViewSliderOptionBox->SetMinValue(60.0f);
	FieldOfViewSliderOptionBox->SetMaxValue(110.0f);
	RenderDistanceSliderOptionBox->SetMinValue(4.0f);
	RenderDistanceSliderOptionBox->SetMaxValue(32.0f);
	RenderDistanceSliderOptionBox->SetShowDecimal(false);
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
	UserSettings->SetRenderDistance(FMath::RoundToInt32(RenderDistanceSliderOptionBox->GetValue()));
	UserSettings->SetShowBranding(ShowBrandingCheckOptionBox->IsChecked());
	UserSettings->SetShowCrosshair(ShowCrosshairCheckOptionBox->IsChecked());
	UserSettings->SetHideChatUnlessOpen(HideChatUnlessOpenCheckOptionBox->IsChecked());
	UserSettings->SetHideHUD(HideHUDCheckOptionBox->IsChecked());
	UserSettings->SetCameraShakeEnabled(CameraShakeEnabledCheckOptionBox->IsChecked());

	ICharacterSettingsInterface* CharacterSettingsInterface = GetOwningPlayerPawn<ICharacterSettingsInterface>();
	if (CharacterSettingsInterface == nullptr)
	{
		return;
	}
	
	CharacterSettingsInterface->ApplyGameplaySettings();

	UserSettings->ApplySettings(false);

	OnRefresh();
}

void UGameplaySettingsWidget::OnRefresh()
{
	Super::OnRefresh();
	
	FieldOfViewSliderOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);
	RenderDistanceSliderOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);
	ShowBrandingCheckOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);
	ShowCrosshairCheckOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);
	HideChatUnlessOpenCheckOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);
	HideHUDCheckOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);
	CameraShakeEnabledCheckOptionBox->OnValueChangedNoParams.RemoveDynamic(this, &UGameplaySettingsWidget::OnApply);

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	FieldOfViewSliderOptionBox->SetValue(UserSettings->GetFieldOfView());
	RenderDistanceSliderOptionBox->SetValue(static_cast<float>(UserSettings->GetRenderDistance()));
	ShowBrandingCheckOptionBox->SetChecked(UserSettings->GetShowBranding());
	ShowCrosshairCheckOptionBox->SetChecked(UserSettings->GetShowCrosshair());
	HideChatUnlessOpenCheckOptionBox->SetChecked(UserSettings->GetHideChatUnlessOpen());
	HideHUDCheckOptionBox->SetChecked(UserSettings->GetHideHUD());
	CameraShakeEnabledCheckOptionBox->SetChecked(UserSettings->GetCameraShakeEnabled());

	FieldOfViewSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	RenderDistanceSliderOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	ShowBrandingCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	ShowCrosshairCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	HideChatUnlessOpenCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	HideHUDCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
	CameraShakeEnabledCheckOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGameplaySettingsWidget::OnApply);
}
