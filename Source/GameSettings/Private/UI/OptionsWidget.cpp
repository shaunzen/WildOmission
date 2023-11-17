// Copyright Telephone Studios. All Rights Reserved.


#include "UI/OptionsWidget.h"
#include "WildOmissionGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Categories/SettingsCategoryWidget.h"
#include "Color/UIColors.h"

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	CategorySwitcher = nullptr;
	AudioSettings = nullptr;
	GameplaySettings = nullptr;
	ControlsSettings = nullptr;
	WindowSettings = nullptr;
	PostProcessingSettings = nullptr;
	GraphicsSettings = nullptr;
	CategoryButtonsVerticalBox = nullptr;
	AudioSettingsButton = nullptr;
	GameplaySettingsButton = nullptr;
	ControlsSettingsButton = nullptr;
	WindowSettingsButton = nullptr;
	PostProcessingSettingsButton = nullptr;
	GraphicsSettingsButton = nullptr;
	ApplyButton = nullptr;
	ResetButton = nullptr;
	BackButton = nullptr;
}

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AudioSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenAudioSettings);
	GameplaySettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenGameplaySettings);
	ControlsSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenControlsSettings);
	WindowSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenWindowSettings);
	PostProcessingSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenPostProcessingSettings);
	GraphicsSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenGraphicsSettings);

	ApplyButton->OnClicked.AddDynamic(this, &UOptionsWidget::Apply);
	ResetButton->OnClicked.AddDynamic(this, &UOptionsWidget::Reset);
	BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::Back);
}

void UOptionsWidget::Refresh()
{
	AudioSettings->OnRefresh();
	GameplaySettings->OnRefresh();
	ControlsSettings->OnRefresh();
	WindowSettings->OnRefresh();
	PostProcessingSettings->OnRefresh();
	GraphicsSettings->OnRefresh();

	RefreshCategoryButtonColor(AudioSettingsButton);
	RefreshCategoryButtonColor(GameplaySettingsButton);
	RefreshCategoryButtonColor(ControlsSettingsButton);
	RefreshCategoryButtonColor(WindowSettingsButton);
	RefreshCategoryButtonColor(PostProcessingSettingsButton);
	RefreshCategoryButtonColor(GraphicsSettingsButton);
}

void UOptionsWidget::OpenAudioSettings()
{
	if (CategorySwitcher == nullptr || AudioSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(AudioSettings);
	Refresh();
}

void UOptionsWidget::OpenGameplaySettings()
{
	if (CategorySwitcher == nullptr || GameplaySettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(GameplaySettings);
	Refresh();
}

void UOptionsWidget::OpenControlsSettings()
{
	if (CategorySwitcher == nullptr || ControlsSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(ControlsSettings);
	Refresh();
}

void UOptionsWidget::OpenWindowSettings()
{

	if (CategorySwitcher == nullptr || WindowSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(WindowSettings);
	Refresh();
}

void UOptionsWidget::OpenPostProcessingSettings()
{
	if (CategorySwitcher == nullptr || PostProcessingSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(PostProcessingSettings);
	Refresh();
}

void UOptionsWidget::OpenGraphicsSettings()
{
	if (CategorySwitcher == nullptr || GraphicsSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(GraphicsSettings);
	Refresh();
}

void UOptionsWidget::RefreshCategoryButtonColor(UButton* ButtonToRefresh)
{
	FUIColor* Blue = UUIColors::GetBaseColor(TEXT("Blue"));
	FUIColor* LightGray = UUIColors::GetBaseColor(TEXT("LightGray"));
	if (Blue == nullptr || LightGray == nullptr || CategorySwitcher == nullptr)
	{
		return;
	}

	const FLinearColor& ColorToSet = CategoryButtonsVerticalBox->GetChildIndex(ButtonToRefresh) == CategorySwitcher->GetActiveWidgetIndex() ? Blue->Default : LightGray->Default;
	ButtonToRefresh->SetBackgroundColor(ColorToSet * FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
}

void UOptionsWidget::Apply()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}
	
	AudioSettings->OnApply();
	GameplaySettings->OnApply();
	ControlsSettings->OnApply();
	WindowSettings->OnApply();
	PostProcessingSettings->OnApply();
	GraphicsSettings->OnApply();

	UserSettings->ApplySettings(false);
	Refresh();
}

void UOptionsWidget::Reset()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetToDefaults();
	Refresh();
}

void UOptionsWidget::Back()
{
	if (!OnBackButtonClicked.IsBound())
	{
		return;
	}

	OnBackButtonClicked.Broadcast();
}