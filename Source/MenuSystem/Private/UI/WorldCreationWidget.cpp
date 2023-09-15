// Copyright Telephone Studios. All Rights Reserved.


#include "WorldCreationWidget.h"
#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "WorldSelectionWidget.h"
#include "Interfaces/GameSaveLoadController.h"
#include "Log.h"

void UWorldCreationWidget::Setup(UMainMenuWidget* InMainMenuParent)
{
	ParentMenu = InMainMenuParent;

	CreateWorldButton->OnClicked.AddDynamic(this, &UWorldCreationWidget::CreateWorld);
	BackButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldSelectionMenu);
	WorldNameInputBox->OnTextChanged.AddDynamic(this, &UWorldCreationWidget::WorldNameOnTextChanged);
}

void UWorldCreationWidget::CreateWorld()
{
	// Get the name of the save
	FString NewWorldName;
	NewWorldName = WorldNameInputBox->GetText().ToString();

	if (NewWorldName == TEXT(""))
	{
		UE_LOG(LogMenuSystem, Warning, TEXT("Cannot create a world without a name."));
		return;
	}

	IGameSaveLoadController* GameSaveLoadController = Cast<IGameSaveLoadController>(GetGameInstance());
	// Create a new world with that name
	GameSaveLoadController->CreateWorld(NewWorldName);

	// To World Menu
	ParentMenu->OpenWorldMenuForWorld(NewWorldName);
}

void UWorldCreationWidget::WorldNameOnTextChanged(const FText& Text)
{
	FString TextString = Text.ToString();

	if (TextString.Len() > 32)
	{
		TextString = TextString.LeftChop(1);
	}

	WorldNameInputBox->SetText(FText::FromString(TextString));
}
