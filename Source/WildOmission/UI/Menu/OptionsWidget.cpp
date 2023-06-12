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

void UOptionsWidget::Open()
{
	// Setup Resolution Settings
	ResolutionScaleOptionBox->ClearOptions();

	ResolutionScaleOptionBox->AddOption(FString(TEXT("20%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("30%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("40%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("50%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("60%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("70%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("80%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("90%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("100%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("110%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("120%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("130%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("140%")));
	ResolutionScaleOptionBox->AddOption(FString(TEXT("150%")));

	UGameUserSettings* UserSettings = GEngine->GameUserSettings;
	float CurrentScaleNormalized = 0.0f;
	int32 CurrentScale = 0;
	int32 MinScale = 0;
	int32 MaxScale = 0;
	UserSettings->GetResolutionScaleInformation(CurrentScaleNormalized, CurrentScale, MinScale, MaxScale);
	FString SelectedScale = FString::Printf(TEXT("%i%"), CurrentScale);
	ResolutionScaleOptionBox->SetSelectedOption(SelectedScale);

}

void UOptionsWidget::Apply()
{
	UGameUserSettings* UserSettings = GEngine->GameUserSettings;


	UserSettings->ApplySettings(false);
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
