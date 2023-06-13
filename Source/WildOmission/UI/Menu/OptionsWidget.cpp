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
	int32 ResolutionScale = UserSettings->GetResolutionScaleAsInt32();

	// Setup Graphics Quality
	GraphicsQualityOptionBox->ClearOptions();
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Low")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Medium")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("High")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Epic")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Cinematic")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Custom")));

	if (OverallGraphicsQuality == -1)
	{
		GraphicsQualityOptionBox->SetSelectedOption(TEXT("Custom"));
	}
	else
	{
		GraphicsQualityOptionBox->SetSelectedIndex(OverallGraphicsQuality);
	}

	// Setup Resolution Settings
	ResolutionScaleOptionBox->ClearOptions();
	ResolutionScaleOptionBox->AddOption(FString(TEXT("50")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("60")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("70")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("80")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("90")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("100")));

	FString SelectedScale = FString::Printf(TEXT("%i"), ResolutionScale);
	ResolutionScaleOptionBox->SetSelectedOption(SelectedScale);

	RefreshGraphicsOptionBoxes(OverallGraphicsQuality == -1);
}

void UOptionsWidget::RefreshGraphicsOptionBoxes(bool IsUsingCustomSettings)
{
	ResolutionScaleOptionBox->SetIsEnabled(IsUsingCustomSettings);
	// TODO other options
}

void UOptionsWidget::ApplyCustomSettings()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();

	// Apply Resolution Scale
	float SelectedResolutionScale = static_cast<float>(FCString::Atoi(*ResolutionScaleOptionBox->GetSelectedOption()));
	UserSettings->SetResolutionScaleValueEx(SelectedResolutionScale);
}

void UOptionsWidget::Apply()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();

	UserSettings->SetFieldOfView(FieldOfViewSlider->GetValue());
	UserSettings->SetMasterVolume(MasterVolumeSlider->GetValue());

	// Apply Graphics Quality
	if (GraphicsQualityOptionBox->GetSelectedOption() != TEXT("Custom"))
	{
		UserSettings->SetOverallScalabilityLevel(GraphicsQualityOptionBox->GetSelectedIndex());
	}
	else if (GraphicsQualityOptionBox->GetSelectedOption() == TEXT("Custom"))
	{
		ApplyCustomSettings();
	}

	UserSettings->ApplySettings(false);

	Refresh();
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
