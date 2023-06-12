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

}

void UOptionsWidget::Setup(UWidget* InParentMenu)
{
	ParentMenu = InParentMenu;

	if (UMainMenuWidget* MainMenu = Cast<UMainMenuWidget>(ParentMenu))
	{
		BackButton->OnClicked.AddDynamic(MainMenu, &UMainMenuWidget::OpenMainMenu);
	}
	else if (UGameplayMenuWidget* GameplayMenu = Cast<UGameplayMenuWidget>(ParentMenu))
	{
		// TODO back to gameplay menu
	}
	ApplyButton->OnClicked.AddDynamic(this, &UOptionsWidget::Apply);
}

void UOptionsWidget::Open()
{
	// Resolution Settings
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
	UE_LOG(LogTemp, Warning, TEXT("Applying Selected Options"));
}
