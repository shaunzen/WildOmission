// Copyright Telephone Studios. All Rights Reserved.


#include "OptionsWidget.h"
#include "MainMenuWidget.h"
#include "GameplayMenuWidget.h"
#include "Components/Button.h"
#include "WildOmission/UI/Custom/SliderOptionBox.h"
#include "WildOmission/UI/Custom/MultiOptionBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WildOmission/Core/WildOmissionGameUserSettings.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

static UWildOmissionGameUserSettings* UserSettings = nullptr;

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();

	FieldOfViewSliderOptionBox->SetMinValue(60.0f);
	FieldOfViewSliderOptionBox->SetMaxValue(110.0f);

	MasterVolumeSliderOptionBox->SetMinValue(0.0f);
	MasterVolumeSliderOptionBox->SetMaxValue(100.0f);

	// Setup Window Mode
	WindowModeOptionBox->ClearOptions();
	WindowModeOptionBox->AddOption(TEXT("Windowed"));
	WindowModeOptionBox->AddOption(TEXT("Windowed Fullscreen"));
	WindowModeOptionBox->AddOption(TEXT("Fullscreen"));

	// Setup Resolution Settings
	FullscreenResolutionOptionBox->ClearOptions();
	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	for (const FIntPoint& Resolution : SupportedResolutions)
	{
		FString ResolutionString = FString::Printf(TEXT("%i X %i"), Resolution.X, Resolution.Y);
		FullscreenResolutionOptionBox->AddOption(ResolutionString);
	}

	// Setup Graphics Quality
	OverallGraphicsQualityOptionBox->GiveQualityOptions();
	OverallGraphicsQualityOptionBox->AddOption(TEXT("Custom"));
	OverallGraphicsQualityOptionBox->OnSelectionChange.AddDynamic(this, &UOptionsWidget::OnOverallQualityOptionChange);


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

	ApplyButton->OnClicked.AddDynamic(this, &UOptionsWidget::Apply);
	ResetButton->OnClicked.AddDynamic(this, &UOptionsWidget::Reset);
	BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::Back);
}

void UOptionsWidget::SetParent(UWidget* InParentMenu)
{
	ParentMenu = InParentMenu;
}

void UOptionsWidget::Refresh()
{
	RefreshGameplaySettings();
	RefreshWindowSettings();
	RefreshGraphicsSettings();
}

void UOptionsWidget::RefreshGameplaySettings()
{
	float FieldOfView = UserSettings->GetFieldOfView();
	float MasterVolume = UserSettings->GetMasterVolume() * 100.0f;

	FieldOfViewSliderOptionBox->SetValue(FieldOfView);
	MasterVolumeSliderOptionBox->SetValue(MasterVolume);
}

void UOptionsWidget::RefreshWindowSettings()
{
	FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
	FString CurrentResolutionString = FString::Printf(TEXT("%i X %i"), CurrentResolution.X, CurrentResolution.Y);
	FullscreenResolutionOptionBox->SetSelectedOption(CurrentResolutionString);
	FullscreenResolutionOptionBox->SetIsEnabled(UserSettings->GetFullscreenMode() == EWindowMode::Type::Fullscreen);

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
}

void UOptionsWidget::RefreshGraphicsSettings()
{
	int32 OverallGraphicsQuality = UserSettings->GetOverallScalabilityLevel();
	bool UsingCustomSettings = OverallGraphicsQuality == -1;
	

	OverallGraphicsQualityOptionBox->SetSelectedIndex(OverallGraphicsQuality);
	if (UsingCustomSettings)
	{
		OverallGraphicsQualityOptionBox->SetSelectedOption(TEXT("Custom"));
	}

	RefreshCustomGraphicsSettings(UsingCustomSettings);
}

void UOptionsWidget::RefreshCustomGraphicsSettings(bool IsUsingCustomSettings)
{
	ViewDistanceQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ShadowQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	GlobalIlluminationQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ReflectionQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	AntiAliasingQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	TextureQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	VisualEffectQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	PostProcessingQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ShaderQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	
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
		int32 SelectedGraphicsQuality = UserSettings->GetOverallScalabilityLevel();

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

void UOptionsWidget::Apply()
{
	UserSettings->SetFieldOfView(FieldOfViewSliderOptionBox->GetValue());
	UserSettings->SetMasterVolume(MasterVolumeSliderOptionBox->GetValue() / 100.0f);

	ApplyWindowSettings();

	// Apply Graphics Quality
	ApplyCustomGraphicsSettings();
	
	if (OverallGraphicsQualityOptionBox->GetSelectedOption() != TEXT("Custom"))
	{
		UserSettings->SetOverallScalabilityLevel(OverallGraphicsQualityOptionBox->GetSelectedIndex());
	}

	UserSettings->ApplySettings(false);
	
	ApplyFieldOfViewSettings();
	ApplyMasterVolumeSettings();

	Refresh();
}

void UOptionsWidget::ApplyWindowSettings()
{
	FString XString;
	FString YString;
	FString SelectedResolutionString = FullscreenResolutionOptionBox->GetSelectedOption();
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
}

void UOptionsWidget::ApplyCustomGraphicsSettings()
{
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

void UOptionsWidget::ApplyFieldOfViewSettings()
{
	UGameplayMenuWidget* GameMenuOwner = Cast<UGameplayMenuWidget>(ParentMenu);
	if (GameMenuOwner == nullptr)
	{
		return;
	}

	AWildOmissionCharacter* Character = GetOwningPlayerPawn<AWildOmissionCharacter>();
	if (Character == nullptr)
	{
		return;
	}
	Character->SetupFieldOfView();
}

void UOptionsWidget::ApplyMasterVolumeSettings()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
	{
		return;
	}

	GameInstance->RefreshMasterVolume();
}

void UOptionsWidget::Reset()
{
	UserSettings->SetToDefaults();
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
		GameplayMenu->OpenGameMenu();
	}
}

void UOptionsWidget::OnOverallQualityOptionChange(const FString& NewSelection)
{
	RefreshCustomGraphicsSettings(NewSelection == TEXT("Custom"));
}