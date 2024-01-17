// Copyright Telephone Studios. All Rights Reserved.


#include "WindowSettingsWidget.h"
#include "OptionBoxes/MultiOptionBox.h"
#include "WildOmissionGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

UWindowSettingsWidget::UWindowSettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	WindowModeOptionBox = nullptr;
	ResolutionOptionBox = nullptr;
	FrameRateLimitOptionBox = nullptr;
}

void UWindowSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Setup Window Mode
	WindowModeOptionBox->ClearOptions();
	WindowModeOptionBox->AddOption(TEXT("Windowed"));
	WindowModeOptionBox->AddOption(TEXT("Windowed Fullscreen"));
	WindowModeOptionBox->AddOption(TEXT("Fullscreen"));

	// Setup Resolution Settings
	ResolutionOptionBox->ClearOptions();
	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	for (const FIntPoint& Resolution : SupportedResolutions)
	{
		FString ResolutionString = FString::Printf(TEXT("%i X %i"), Resolution.X, Resolution.Y);
		ResolutionOptionBox->AddOption(ResolutionString);
	}

	// Setup Frame Rate Limit Settings
	FrameRateLimitOptionBox->ClearOptions();
	FrameRateLimitOptionBox->AddOption(TEXT("Unlimited"));
	FrameRateLimitOptionBox->AddOption(TEXT("30"));
	FrameRateLimitOptionBox->AddOption(TEXT("60"));
	FrameRateLimitOptionBox->AddOption(TEXT("100"));
	FrameRateLimitOptionBox->AddOption(TEXT("120"));
	FrameRateLimitOptionBox->AddOption(TEXT("144"));
	FrameRateLimitOptionBox->AddOption(TEXT("240"));

	WindowModeOptionBox->OnValueChangedNoParams.AddDynamic(this, &UWindowSettingsWidget::OnApply);
	ResolutionOptionBox->OnValueChangedNoParams.AddDynamic(this, &UWindowSettingsWidget::OnApply);
	FrameRateLimitOptionBox->OnValueChangedNoParams.AddDynamic(this, &UWindowSettingsWidget::OnApply);
}

void UWindowSettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	FString XString;
	FString YString;
	FString SelectedResolutionString = ResolutionOptionBox->GetSelectedOption();
	SelectedResolutionString.Split(TEXT(" X "), &XString, &YString);
	FIntPoint NewResolution = FIntPoint::ZeroValue;
	NewResolution.X = FCString::Atoi(*XString);
	NewResolution.Y = FCString::Atoi(*YString);
	UserSettings->SetScreenResolution(NewResolution);

	switch (WindowModeOptionBox->GetSelectedIndex())
	{
	case 0:
		UserSettings->SetFullscreenMode(EWindowMode::Type::Windowed);
		break;
	case 1:
		UserSettings->SetFullscreenMode(EWindowMode::Type::WindowedFullscreen);
		break;
	case 2:
		UserSettings->SetFullscreenMode(EWindowMode::Type::Fullscreen);
		break;
	}

	switch (FrameRateLimitOptionBox->GetSelectedIndex())
	{
	case 0:
		UserSettings->SetFrameRateLimit(0.0f);
		break;
	case 1:
		UserSettings->SetFrameRateLimit(30.0f);
		break;
	case 2:
		UserSettings->SetFrameRateLimit(60.0f);
		break;
	case 3:
		UserSettings->SetFrameRateLimit(100.0f);
		break;
	case 4:
		UserSettings->SetFrameRateLimit(120.0f);
		break;
	case 5:
		UserSettings->SetFrameRateLimit(144.0f);
		break;
	case 6:
		UserSettings->SetFrameRateLimit(240.0f);
		break;
	}

	UserSettings->ApplySettings(false);

	OnRefresh();
}

void UWindowSettingsWidget::OnRefresh()
{
	Super::OnRefresh();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
	FString CurrentResolutionString = FString::Printf(TEXT("%i X %i"), CurrentResolution.X, CurrentResolution.Y);
	ResolutionOptionBox->SetSelectedOption(CurrentResolutionString);
	ResolutionOptionBox->SetIsEnabled(UserSettings->GetFullscreenMode() == EWindowMode::Type::Fullscreen || UserSettings->GetFullscreenMode() == EWindowMode::Type::Windowed);

	switch (UserSettings->GetFullscreenMode())
	{
	case EWindowMode::Type::Windowed:
		WindowModeOptionBox->SetSelectedIndex(0);
		break;
	case EWindowMode::Type::WindowedFullscreen:
		WindowModeOptionBox->SetSelectedIndex(1);
		break;
	case EWindowMode::Type::Fullscreen:
		WindowModeOptionBox->SetSelectedIndex(2);
		break;
	}

	switch (FMath::RoundToInt32(UserSettings->GetFrameRateLimit()))
	{
	case 0:
		FrameRateLimitOptionBox->SetSelectedOption(TEXT("Unlimited"));
		break;
	default:
		FString FrameLimitString = FString::Printf(TEXT("%i"), FMath::RoundToInt32(UserSettings->GetFrameRateLimit()));
		FrameRateLimitOptionBox->SetSelectedOption(FrameLimitString);
		break;
	}
}
