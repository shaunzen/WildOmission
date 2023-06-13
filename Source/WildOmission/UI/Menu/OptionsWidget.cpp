// Copyright Telephone Studios. All Rights Reserved.


#include "OptionsWidget.h"
#include "MainMenuWidget.h"
#include "GameplayMenuWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "WildOmission/UI/Custom/MultiOptionBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WildOmission/Core/WildOmissionGameUserSettings.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ApplyButton->OnClicked.AddDynamic(this, &UOptionsWidget::Apply);
	BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::Back);

	// Setup Graphics Quality
	OverallGraphicsQualityOptionBox->GiveQualityOptions();
	OverallGraphicsQualityOptionBox->AddOption(TEXT("Custom"));
	OverallGraphicsQualityOptionBox->OnSelectionChange.AddDynamic(this, &UOptionsWidget::CheckOverallScalabilityCustom);

	// Setup Resolution Settings
	ResolutionScaleOptionBox->ClearOptions();
	ResolutionScaleOptionBox->AddOption(TEXT("50"));
	ResolutionScaleOptionBox->AddOption(TEXT("60"));
	ResolutionScaleOptionBox->AddOption(TEXT("70"));
	ResolutionScaleOptionBox->AddOption(TEXT("80"));
	ResolutionScaleOptionBox->AddOption(TEXT("90"));
	ResolutionScaleOptionBox->AddOption(TEXT("100"));

	// Setup Custom Settings
	ViewDistanceQualityOptionBox->GiveQualityOptions();
	ShadowQualityOptionBox->GiveQualityOptions();
	GlobalIlluminationQualityOptionBox->GiveQualityOptions();
	ReflectionQualityOptionBox->GiveQualityOptions();
	AntiAliasingQualityOptionBox->GiveQualityOptions();
	TextureQualityOptionBox->GiveQualityOptions();
	VisualEffectQualityOptionBox->GiveQualityOptions();
	PostProcessingQualityOptionBox->GiveQualityOptions();
	ShaderQualityOptionBox->GiveQualityOptions();
}

void UOptionsWidget::Setup(UWidget* InParentMenu)
{
	ParentMenu = InParentMenu;
}

void UOptionsWidget::Refresh()
{
	RefreshGameplaySettings();
	RefreshGraphicsSettings();
}

void UOptionsWidget::RefreshGameplaySettings()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	float FieldOfView = UserSettings->GetFieldOfView();
	float MasterVolume = UserSettings->GetMasterVolume();

	FieldOfViewSlider->SetValue(FieldOfView);
	MasterVolumeSlider->SetValue(MasterVolume);
}

void UOptionsWidget::RefreshGraphicsSettings()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	int32 OverallGraphicsQuality = UserSettings->GetOverallScalabilityLevel();
	bool UsingCustomSettings = OverallGraphicsQuality == -1;

	OverallGraphicsQualityOptionBox->SetSelectedIndex(OverallGraphicsQuality);
	if (UsingCustomSettings)
	{
		OverallGraphicsQualityOptionBox->SetSelectedOption(TEXT("Custom"));
	}

	RefreshCustomGraphicsOptionBoxes(UsingCustomSettings);
}

void UOptionsWidget::RefreshCustomGraphicsOptionBoxes(bool IsUsingCustomSettings)
{
	ResolutionScaleOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ViewDistanceQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ShadowQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	GlobalIlluminationQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ReflectionQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	AntiAliasingQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	TextureQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	VisualEffectQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	PostProcessingQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ShaderQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	FString ResolutionScale = FString::Printf(TEXT("%i"), UserSettings->GetResolutionScaleAsInt32());
	ResolutionScaleOptionBox->SetSelectedOption(ResolutionScale);
	ViewDistanceQualityOptionBox->SetSelectedIndex(UserSettings->GetViewDistanceQuality());
	ShadowQualityOptionBox->SetSelectedIndex(UserSettings->GetShadowQuality());
	GlobalIlluminationQualityOptionBox->SetSelectedIndex(UserSettings->GetGlobalIlluminationQuality());
	ReflectionQualityOptionBox->SetSelectedIndex(UserSettings->GetReflectionQuality());
	AntiAliasingQualityOptionBox->SetSelectedIndex(UserSettings->GetAntiAliasingQuality());
	TextureQualityOptionBox->SetSelectedIndex(UserSettings->GetTextureQuality());
	VisualEffectQualityOptionBox->SetSelectedIndex(UserSettings->GetVisualEffectQuality());
	PostProcessingQualityOptionBox->SetSelectedIndex(UserSettings->GetPostProcessingQuality());
	ShaderQualityOptionBox->SetSelectedIndex(UserSettings->GetShadingQuality());

	if (!IsUsingCustomSettings)
	{
		int32 SelectedGraphicsQuality = OverallGraphicsQualityOptionBox->GetSelectedIndex();
		switch (SelectedGraphicsQuality)
		{
		case 0: // Low
			ResolutionScaleOptionBox->SetSelectedOption(TEXT("50"));
			break;
		case 1: // Med
			ResolutionScaleOptionBox->SetSelectedOption(TEXT("50"));
			break;
		case 2:	// High
			ResolutionScaleOptionBox->SetSelectedOption(TEXT("50"));
			break;
		case 3: // Epic
			ResolutionScaleOptionBox->SetSelectedOption(TEXT("100"));
			break;
		case 4:	// Cinematic
			ResolutionScaleOptionBox->SetSelectedOption(TEXT("100"));
			break;
		}
		ResolutionScaleOptionBox->SetSelectedOption(ResolutionScale);
		ViewDistanceQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		ShadowQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		GlobalIlluminationQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		ReflectionQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		AntiAliasingQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		TextureQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		VisualEffectQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		PostProcessingQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
		ShaderQualityOptionBox->SetSelectedIndex(SelectedGraphicsQuality);
	}
}

void UOptionsWidget::CheckOverallScalabilityCustom(const FString& NewSelection)
{
	RefreshCustomGraphicsOptionBoxes(NewSelection == TEXT("Custom"));
}

void UOptionsWidget::Apply()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();

	UserSettings->SetFieldOfView(FieldOfViewSlider->GetValue());
	UserSettings->SetMasterVolume(MasterVolumeSlider->GetValue());

	// Apply Graphics Quality
	ApplyCustomGraphicsSettings();
	
	if (OverallGraphicsQualityOptionBox->GetSelectedOption() != TEXT("Custom"))
	{
		UserSettings->SetOverallScalabilityLevel(OverallGraphicsQualityOptionBox->GetSelectedIndex());
	}

	UserSettings->ApplySettings(false);

	Refresh();
}

void UOptionsWidget::ApplyCustomGraphicsSettings()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	float SelectedResolutionScale = FCString::Atof(*ResolutionScaleOptionBox->GetSelectedOption());

	UserSettings->SetResolutionScaleValueEx(SelectedResolutionScale);
	UserSettings->SetViewDistanceQuality(ViewDistanceQualityOptionBox->GetSelectedIndex());
	UserSettings->SetShadowQuality(ShadowQualityOptionBox->GetSelectedIndex());
	UserSettings->SetGlobalIlluminationQuality(GlobalIlluminationQualityOptionBox->GetSelectedIndex());
	UserSettings->SetReflectionQuality(ReflectionQualityOptionBox->GetSelectedIndex());
	UserSettings->SetAntiAliasingQuality(AntiAliasingQualityOptionBox->GetSelectedIndex());
	UserSettings->SetTextureQuality(TextureQualityOptionBox->GetSelectedIndex());
	UserSettings->SetVisualEffectQuality(VisualEffectQualityOptionBox->GetSelectedIndex());
	UserSettings->SetPostProcessingQuality(PostProcessingQualityOptionBox->GetSelectedIndex());
	UserSettings->SetShadingQuality(ShaderQualityOptionBox->GetSelectedIndex());
}

void UOptionsWidget::Back()
{
	UMainMenuWidget* MainMenu = Cast<UMainMenuWidget>(ParentMenu);
	UGameplayMenuWidget* GameplayMenu = Cast<UGameplayMenuWidget>(ParentMenu);
	if (MainMenu)
	{
		MainMenu->OpenMainMenu();
	}
	else if (GameplayMenu)
	{
		// TODO back to gameplay menu
	}
}
