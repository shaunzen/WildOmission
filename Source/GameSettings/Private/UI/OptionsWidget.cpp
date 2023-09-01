// Copyright Telephone Studios. All Rights Reserved.


#include "UI/OptionsWidget.h"
#include "WildOmissionGameUserSettings.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Categories/SettingsCategoryWidget.h"

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	GameplaySettingsButton = nullptr;
	WindowSettingsButton = nullptr;
	GraphicsSettingsButton = nullptr;
	ApplyButton = nullptr;
	ResetButton = nullptr;
	BackButton = nullptr;
}

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameplaySettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenGameplaySettings);
	WindowSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenWindowSettings);
	GraphicsSettingsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OpenGraphicsSettings);

	ApplyButton->OnClicked.AddDynamic(this, &UOptionsWidget::Apply);
	ResetButton->OnClicked.AddDynamic(this, &UOptionsWidget::Reset);
	BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::Back);
}

void UOptionsWidget::Refresh()
{
	GameplaySettings->OnRefresh();
	WindowSettings->OnRefresh();
	GraphicsSettings->OnRefresh();
}

void UOptionsWidget::OpenGameplaySettings()
{
	if (CategorySwitcher == nullptr || GameplaySettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(GameplaySettings);
	GameplaySettings->OnRefresh();
}

void UOptionsWidget::OpenWindowSettings()
{

	if (CategorySwitcher == nullptr || WindowSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(WindowSettings);
	WindowSettings->OnRefresh();
}

void UOptionsWidget::OpenGraphicsSettings()
{

	if (CategorySwitcher == nullptr || GraphicsSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(GraphicsSettings);
	GraphicsSettings->OnRefresh();
}

void UOptionsWidget::Apply()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}
	
	GameplaySettings->OnApply();
	WindowSettings->OnApply();
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
	if (!OnBackButtonPressed.IsBound())
	{
		return;
	}

	OnBackButtonPressed.Broadcast();
}