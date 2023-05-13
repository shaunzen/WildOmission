// Copyright Telephone Studios. All Rights Reserved.


#include "CreateWorldWidget.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "WorldSelectionWidget.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"

void UCreateWorldWidget::Setup(UMainMenuWidget* InMainMenuParent)
{
	ParentMenu = InMainMenuParent;

	CreateWorldButton->OnClicked.AddDynamic(this, &UCreateWorldWidget::CreateWorld);
	BackButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldSelectionMenu);
	WorldNameInputBox->OnTextChanged.AddDynamic(this, &UCreateWorldWidget::WorldNameOnTextChanged);
}

void UCreateWorldWidget::CreateWorld()
{
	// Get the name of the save
	FString NewWorldName;
	NewWorldName = WorldNameInputBox->GetText().ToString();

	if (NewWorldName == FString(""))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create a world without a name."));
		return;
	}

	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	// Create a new world with that name
	GameInstance->CreateWorld(NewWorldName);

	// To World Menu
	ParentMenu->OpenWorldMenuForWorld(NewWorldName);
}

void UCreateWorldWidget::WorldNameOnTextChanged(const FText& Text)
{
	FString TextString = Text.ToString();

	if (TextString.Len() > 16)
	{
		TextString = TextString.LeftChop(1);
	}

	WorldNameInputBox->SetText(FText::FromString(TextString));
}
