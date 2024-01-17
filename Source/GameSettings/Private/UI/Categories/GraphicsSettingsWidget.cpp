// Copyright Telephone Studios. All Rights Reserved.


#include "GraphicsSettingsWidget.h"
#include "Components/Button.h"
#include "OptionBoxes/MultiOptionBox.h"
#include "WildOmissionGameUserSettings.h"

UGraphicsSettingsWidget::UGraphicsSettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	RunHardwareBenchmarkButton = nullptr;
	OverallGraphicsQualityOptionBox = nullptr;
	ViewDistanceQualityOptionBox = nullptr;
	ShadowQualityOptionBox = nullptr;
	GlobalIlluminationQualityOptionBox = nullptr;
	ReflectionQualityOptionBox = nullptr;
	AntiAliasingQualityOptionBox = nullptr;
	TextureQualityOptionBox = nullptr;
	VisualEffectQualityOptionBox = nullptr;
	PostProcessingQualityOptionBox = nullptr;
	FoliageQualityOptionBox = nullptr;
	ShaderQualityOptionBox = nullptr;
}

void UGraphicsSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RunHardwareBenchmarkButton->OnClicked.AddDynamic(this, &UGraphicsSettingsWidget::RunHardwareBenchmark);

	// Setup Graphics Quality
	OverallGraphicsQualityOptionBox->GiveQualityOptions();
	OverallGraphicsQualityOptionBox->AddOption(TEXT("Custom"));
	OverallGraphicsQualityOptionBox->OnValueChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnOverallQualityOptionChange);

	// Setup Custom Settings
	ViewDistanceQualityOptionBox->GiveQualityOptions();
	ShadowQualityOptionBox->GiveQualityOptions();
	GlobalIlluminationQualityOptionBox->GiveQualityOptions();
	ReflectionQualityOptionBox->GiveQualityOptions();
	AntiAliasingQualityOptionBox->GiveQualityOptions();
	TextureQualityOptionBox->GiveQualityOptions();
	VisualEffectQualityOptionBox->GiveQualityOptions();
	PostProcessingQualityOptionBox->GiveQualityOptions();
	FoliageQualityOptionBox->GiveQualityOptions();
	ShaderQualityOptionBox->GiveQualityOptions();

	OverallGraphicsQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	ViewDistanceQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	ShadowQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	GlobalIlluminationQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	ReflectionQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	AntiAliasingQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	TextureQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	VisualEffectQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	PostProcessingQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	FoliageQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
	ShaderQualityOptionBox->OnValueChangedNoParams.AddDynamic(this, &UGraphicsSettingsWidget::OnApply);
}

void UGraphicsSettingsWidget::OnApply()
{
	Super::OnApply();
	
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetViewDistanceQuality(ViewDistanceQualityOptionBox->GetSelectedIndex());
	UserSettings->SetShadowQuality(ShadowQualityOptionBox->GetSelectedIndex());
	UserSettings->SetGlobalIlluminationQuality(GlobalIlluminationQualityOptionBox->GetSelectedIndex());
	UserSettings->SetReflectionQuality(ReflectionQualityOptionBox->GetSelectedIndex());
	UserSettings->SetAntiAliasingQuality(AntiAliasingQualityOptionBox->GetSelectedIndex());
	UserSettings->SetTextureQuality(TextureQualityOptionBox->GetSelectedIndex());
	UserSettings->SetVisualEffectQuality(VisualEffectQualityOptionBox->GetSelectedIndex());
	UserSettings->SetPostProcessingQuality(PostProcessingQualityOptionBox->GetSelectedIndex());
	UserSettings->SetFoliageQuality(FoliageQualityOptionBox->GetSelectedIndex());
	UserSettings->SetShadingQuality(ShaderQualityOptionBox->GetSelectedIndex());

	if (OverallGraphicsQualityOptionBox->GetSelectedOption() != TEXT("Custom"))
	{
		UserSettings->SetOverallScalabilityLevel(OverallGraphicsQualityOptionBox->GetSelectedIndex());
	}

	UserSettings->ApplySettings(false);
}

void UGraphicsSettingsWidget::OnRefresh()
{
	Super::OnRefresh();
	
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	int32 OverallGraphicsQuality = UserSettings->GetOverallScalabilityLevel();
	bool UsingCustomSettings = OverallGraphicsQuality == -1;

	OverallGraphicsQualityOptionBox->SetSelectedIndex(OverallGraphicsQuality);
	if (UsingCustomSettings)
	{
		OverallGraphicsQualityOptionBox->SetSelectedOption(TEXT("Custom"));
	}

	RefreshCustomGraphicsSettings(UsingCustomSettings);
}

void UGraphicsSettingsWidget::RefreshCustomGraphicsSettings(bool IsUsingCustomSettings)
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	ViewDistanceQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ShadowQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	GlobalIlluminationQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ReflectionQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	AntiAliasingQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	TextureQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	VisualEffectQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	PostProcessingQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	FoliageQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);
	ShaderQualityOptionBox->SetIsEnabled(IsUsingCustomSettings);

	ViewDistanceQualityOptionBox->SetSelectedIndex(UserSettings->GetViewDistanceQuality());
	ShadowQualityOptionBox->SetSelectedIndex(UserSettings->GetShadowQuality());
	GlobalIlluminationQualityOptionBox->SetSelectedIndex(UserSettings->GetGlobalIlluminationQuality());
	ReflectionQualityOptionBox->SetSelectedIndex(UserSettings->GetReflectionQuality());
	AntiAliasingQualityOptionBox->SetSelectedIndex(UserSettings->GetAntiAliasingQuality());
	TextureQualityOptionBox->SetSelectedIndex(UserSettings->GetTextureQuality());
	VisualEffectQualityOptionBox->SetSelectedIndex(UserSettings->GetVisualEffectQuality());
	PostProcessingQualityOptionBox->SetSelectedIndex(UserSettings->GetPostProcessingQuality());
	FoliageQualityOptionBox->SetSelectedIndex(UserSettings->GetFoliageQuality());
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

void UGraphicsSettingsWidget::OnOverallQualityOptionChange(const FString& NewSelection)
{
	RefreshCustomGraphicsSettings(NewSelection == TEXT("Custom"));
}

void UGraphicsSettingsWidget::RunHardwareBenchmark()
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->RunHardwareBenchmark(10, 1.0f, 0.2f);
	UserSettings->ApplyHardwareBenchmarkResults();
	OnRefresh();
}
