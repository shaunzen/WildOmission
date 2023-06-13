// Copyright Telephone Studios. All Rights Reserved.


#include "OptionsWidget.h"
#include "MainMenuWidget.h"
#include "GameplayMenuWidget.h"
#include "Components/Button.h"
#include "WildOmission/UI/Custom/MultiOptionBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"

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
	UGameUserSettings* UserSettings = GEngine->GameUserSettings;

	// Setup Resolution Settings
	ResolutionScaleOptionBox->ClearOptions();
	ResolutionScaleOptionBox->AddOption(FString(TEXT("50")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("60")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("70")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("80")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("90")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("100")));
	
	float CurrentScaleNormalized = 0.0f;
	float CurrentScale = 0.0f;
	float MinScale = 0.0f;
	float MaxScale = 0.0f;
	UserSettings->GetResolutionScaleInformationEx(CurrentScaleNormalized, CurrentScale, MinScale, MaxScale);
	FString SelectedScale = FString::Printf(TEXT("%i"), CurrentScale);
	ResolutionScaleOptionBox->SetSelectedOption(SelectedScale);

	// Setup Graphics Quality
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Low")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Medium")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("High")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Epic")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Cinematic")));
	GraphicsQualityOptionBox->AddOption(FString(TEXT("Custom")));
	// Check if custom
	if (UserSettings->GetOverallScalabilityLevel() == -1)
	{
		GraphicsQualityOptionBox->SetSelectedOption(FString("Custom"));
	}
	else
	{
		GraphicsQualityOptionBox->SetSelectedIndex(UserSettings->GetOverallScalabilityLevel());
	}

}

void UOptionsWidget::Apply()
{
	UGameUserSettings* UserSettings = GEngine->GameUserSettings;

	// Apply Resolution Scale
	float SelectedResolutionScale = FCString::Atof(*ResolutionScaleOptionBox->GetSelectedOption());
	UserSettings->SetResolutionScaleValueEx(SelectedResolutionScale);

	// Apply Graphics Quality
	UserSettings->SetOverallScalabilityLevel(GraphicsQualityOptionBox->GetSelectedIndex());

	UserSettings->ApplySettings(false);

	Refresh();
	UE_LOG(LogTemp, Warning, TEXT("Applying Selected Options"));
}

void UOptionsWidget::Back()
{
	UE_LOG(LogTemp, Warning, TEXT("Back Clicked."));
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
