// Copyright Telephone Studios. All Rights Reserved.


#include "WorldCreationWidget.h"
#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "WorldSelectionWidget.h"
#include "Interfaces/GameSaveLoadController.h"
#include "Log.h"

UWorldCreationWidget::UWorldCreationWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	CreateWorldButton = nullptr;
	BackButton = nullptr;
	WorldNameInputBox = nullptr;
	
	InvalidWarningBorder = nullptr;
	InvalidWarningTextBlock = nullptr;
	
	ParentMenu = nullptr;
}

void UWorldCreationWidget::Setup(UMainMenuWidget* InMainMenuParent)
{
	ParentMenu = InMainMenuParent;

	HideInvalidWarning();

	CreateWorldButton->OnClicked.AddDynamic(this, &UWorldCreationWidget::CreateWorld);
	CreateWorldButton->SetIsEnabled(false);
	BackButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldSelectionMenu);
	WorldNameInputBox->OnTextChanged.AddDynamic(this, &UWorldCreationWidget::WorldNameOnTextChanged);
}

void UWorldCreationWidget::ShowInvalidWarning(const FString& Warning)
{
	InvalidWarningTextBlock->SetText(FText::FromString(Warning));

	InvalidWarningBorder->SetVisibility(ESlateVisibility::Visible);
}

void UWorldCreationWidget::HideInvalidWarning()
{
	InvalidWarningBorder->SetVisibility(ESlateVisibility::Collapsed);
}

bool UWorldCreationWidget::WorldOfSameNameAlreadyExists(const FString& WorldName)
{
	return IMenuInterface::WorldAlreadyExists(WorldName);
}

bool UWorldCreationWidget::WorldContainsInvalidCharacter(const FString& WorldName)
{
	if (WorldName.Contains(TEXT("CON")) || WorldName.Contains(TEXT("PRN")) ||
		WorldName.Contains(TEXT("AUX")) || WorldName.Contains(TEXT("NUL")) ||
		WorldName.Contains(TEXT("COM1")) || WorldName.Contains(TEXT("COM2")) ||
		WorldName.Contains(TEXT("COM3")) || WorldName.Contains(TEXT("COM4")) ||
		WorldName.Contains(TEXT("COM5")) || WorldName.Contains(TEXT("COM6")) ||
		WorldName.Contains(TEXT("COM7")) || WorldName.Contains(TEXT("COM8")) ||
		WorldName.Contains(TEXT("COM9")) || WorldName.Contains(TEXT("COM0")) ||
		WorldName.Contains(TEXT("<")) || WorldName.Contains(TEXT(">")) ||
		WorldName.Contains(TEXT(":")) || WorldName.Contains(TEXT("\"")) ||
		WorldName.Contains(TEXT("/")) || WorldName.Contains(TEXT("\\")) ||
		WorldName.Contains(TEXT("?")) || WorldName.Contains(TEXT("*")))
	{
		return true;
	}

	return false;
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

	if (TextString.Len() >= 0)
	{
		CreateWorldButton->SetIsEnabled(false);
	}

	if (WorldOfSameNameAlreadyExists(TextString) || WorldContainsInvalidCharacter(TextString))
	{
		CreateWorldButton->SetIsEnabled(false);
	}
	else
	{
		CreateWorldButton->SetIsEnabled(true);
		HideInvalidWarning();
	}
	
	
}
