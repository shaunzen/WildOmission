// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/MainMenuWidget.h"

UWildOmissionGameInstance::UWildOmissionGameInstance(const FObjectInitializer& ObjectIntializer)
{
	ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuBlueprint(TEXT("/Game/Blueprints/MenuSystem/WBP_MainMenu"));
	if (MainMenuBlueprint.Class)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu widget blueprint"));
		MainMenuWidgetBlueprintClass = MainMenuBlueprint.Class;
	}
}

void UWildOmissionGameInstance::ShowMainMenuWidget()
{
	if (MainMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create the main menu widget, blueprint class was nullptr"));
		return;
	}
	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetBlueprintClass);
	if (MainMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create the main menu widget"))
		return;
	}

	MainMenuWidget->Setup();
}

void UWildOmissionGameInstance::StartSession()
{
	UE_LOG(LogTemp, Display, TEXT("Starting Session"));
}

void UWildOmissionGameInstance::RefreshServerList()
{
	UE_LOG(LogTemp, Display, TEXT("Refreshing Server List"));
}

//****************************
// HOSTING/JOINING
//****************************
void UWildOmissionGameInstance::Host(FString ServerName)
{
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server Named: %s"), *ServerName);
}

void UWildOmissionGameInstance::Join(uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Joining Server Index: %i"), Index);
}