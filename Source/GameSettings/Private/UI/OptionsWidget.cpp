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
	CategoryButtonsVerticalBox = nullptr;
	GameplaySettingsButton = nullptr;
	WindowSettingsButton = nullptr;
	GraphicsSettingsButton = nullptr;
	CategorySwitcher = nullptr;
	GameplaySettings = nullptr;
	WindowSettings = nullptr;
	GraphicsSettings = nullptr;
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
	RefreshAllCategoryButtons();
}

void UOptionsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

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

void UOptionsWidget::OpenWindowSettings()
{

	if (CategorySwitcher == nullptr || WindowSettings == nullptr)
	{
		return;
	}

	CategorySwitcher->SetActiveWidget(WindowSettings);
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

void UOptionsWidget::RefreshAllCategoryButtons()
{
	RefreshCategoryButtonColor(GameplaySettingsButton);
	RefreshCategoryButtonColor(WindowSettingsButton);
	RefreshCategoryButtonColor(GraphicsSettingsButton);
}

void UOptionsWidget::RefreshCategoryButtonColor(UButton* ButtonToRefresh)
{
	FUIColor* Blue = UUIColors::GetBaseColor(TEXT("Blue"));
	FUIColor* LightGray = UUIColors::GetBaseColor(TEXT("LightGray"));
	if (Blue == nullptr || LightGray == nullptr || CategorySwitcher == nullptr)
	{
		return;
	}

	const FLinearColor& ColorToSet = CategoryButtonsVerticalBox->GetChildIndex(ButtonToRefresh) == CategorySwitcher->ActiveWidgetIndex ? Blue->Default : LightGray->Default;
	ButtonToRefresh->SetBackgroundColor(ColorToSet);
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