// Copyright Telephone Studios. All Rights Reserved.


#include "WorldMenuWidget.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "GameFramework/PlayerState.h"

void UWorldMenuWidget::Setup(UMainMenuWidget* InMainMenuParent)
{
	ParentMenu = InMainMenuParent;

	PlayButton->OnClicked.AddDynamic(this, &UWorldMenuWidget::PlayButtonClicked);
	BackButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldSelectionMenu);
	MultiplayerCheckBox->OnCheckStateChanged.AddDynamic(this, &UWorldMenuWidget::MultiplayerCheckboxChanged);
	ServerNameInputBox->OnTextChanged.AddDynamic(this, &UWorldMenuWidget::ServerNameOnTextChanged);
}

void UWorldMenuWidget::Open(const FString& InWorldName)
{
	WorldName = InWorldName;

	Title->SetText(FText::FromString(WorldName));

	// Setting Placeholder Server Name
	FString PlaceholderServerName;
	APlayerState* PlayerState = GetOwningPlayerState();
	if (PlayerState == nullptr)
	{
		return;
	}
	PlaceholderServerName = FString::Printf(TEXT("%s's Server"), *PlayerState->GetPlayerName());
	ServerNameInputBox->SetText(FText::FromString(PlaceholderServerName));
}

void UWorldMenuWidget::ServerNameOnTextChanged(const FText& Text)
{
	FString TextString = Text.ToString();

	if (TextString.Len() > 32)
	{
		TextString = TextString.LeftChop(1);
	}

	ServerNameInputBox->SetText(FText::FromString(TextString));
}

void UWorldMenuWidget::MultiplayerCheckboxChanged(bool bIsChecked)
{
	HostSettingsMenu->SetIsEnabled(bIsChecked);
}

void UWorldMenuWidget::PlayButtonClicked()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());

	if (GameInstance == nullptr || WorldName == FString())
	{
		return;
	}

	if (MultiplayerCheckBox->IsChecked())
	{
		FString ServerName = ServerNameInputBox->GetText().ToString();
		if (ServerName == FString(""))
		{
			return;
		}

		GameInstance->Host(ServerName, WorldName);
	}
	else
	{
		GameInstance->StartSingleplayer(WorldName);
	}
}