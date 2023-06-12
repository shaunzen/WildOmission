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
	ResolutionOptionBox->ClearOptions();
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	for (const FIntPoint& Resolution : Resolutions)
	{
		FString OptionString = FString::Printf(TEXT("%i x %i"), Resolution.X, Resolution.Y);

		ResolutionOptionBox->AddOption(OptionString);
	}
	UGameUserSettings* UserSettings = GEngine->GameUserSettings;
	FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
	FString CurrentResolutionString = FString::Printf(TEXT("%i x %i"), CurrentResolution.X, CurrentResolution.Y);

	ResolutionOptionBox->SetSelectedOption(CurrentResolutionString);
}

void UOptionsWidget::Apply()
{
	UGameUserSettings* UserSettings = GEngine->GameUserSettings;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	FString XString;
	FString YString;
	FString Resolution = ResolutionOptionBox->GetSelectedOption();
	Resolution.Split(FString(TEXT(" x ")), &XString, &YString);

	FIntPoint NewResolution;
	NewResolution.X = FCString::Atoi(*XString);
	NewResolution.Y = FCString::Atoi(*YString);
	UserSettings->SetScreenResolution(NewResolution);

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
