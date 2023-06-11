// Copyright Telephone Studios. All Rights Reserved.


#include "OptionsWidget.h"
#include "MainMenuWidget.h"
#include "GameplayMenuWidget.h"
#include "Components/Button.h"

void UOptionsWidget::NativeConstruct()
{

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

void UOptionsWidget::Apply()
{
	UE_LOG(LogTemp, Warning, TEXT("Applying Selected Options"));
}
