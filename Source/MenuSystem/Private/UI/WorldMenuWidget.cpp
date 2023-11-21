// Copyright Telephone Studios. All Rights Reserved.


#include "WorldMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

UWorldMenuWidget::UWorldMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Title = nullptr;
	PlayButton = nullptr;
	RenameButton = nullptr;
	DeleteButton = nullptr;
	CancelButton = nullptr;
	MultiplayerCheckOptionBox = nullptr;
	FriendsOnlyCheckOptionBox = nullptr;
	MaxPlayersSliderOptionBox = nullptr;
	HostSettingsMenu = nullptr;
	ServerNameInputBox = nullptr;

	WorldName = TEXT("");
}

void UWorldMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MaxPlayersSliderOptionBox->SetMaxValue(16);
	MaxPlayersSliderOptionBox->SetMinValue(2);
	MaxPlayersSliderOptionBox->SetValue(8);

	PlayButton->OnClicked.AddDynamic(this, &UWorldMenuWidget::BroadcastPlayButtonClicked);
	RenameButton->OnClicked.AddDynamic(this, &UWorldMenuWidget::BroadcastRenameButtonClicked);
	DeleteButton->OnClicked.AddDynamic(this, &UWorldMenuWidget::BroadcastDeleteButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UWorldMenuWidget::BroadcastCancelButtonClicked);
	MultiplayerCheckOptionBox->OnCheckStateChanged.AddDynamic(this, &UWorldMenuWidget::MultiplayerCheckboxChanged);
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

void UWorldMenuWidget::BroadcastPlayButtonClicked()
{
	if (!OnPlayButtonClicked.IsBound() 
		|| WorldName.IsEmpty())
	{
		return;
	}
	
	const FString ServerName = ServerNameInputBox->GetText().ToString();
	const bool IsMultiplayer = MultiplayerCheckOptionBox->IsChecked();
	const bool IsFriendsOnly = FriendsOnlyCheckOptionBox->IsChecked();

	OnPlayButtonClicked.Broadcast(WorldName, ServerName, IsMultiplayer, IsFriendsOnly, FMath::RoundToInt32(MaxPlayersSliderOptionBox->GetValue()));
}

void UWorldMenuWidget::BroadcastRenameButtonClicked()
{
	if (!OnRenameButtonClicked.IsBound())
	{
		return;
	}

	OnRenameButtonClicked.Broadcast();
}

void UWorldMenuWidget::BroadcastDeleteButtonClicked()
{
	if (!OnDeleteButtonClicked.IsBound())
	{
		return;
	}

	OnDeleteButtonClicked.Broadcast();
}

void UWorldMenuWidget::BroadcastCancelButtonClicked()
{
	if (!OnCancelButtonClicked.IsBound())
	{
		return;
	}

	OnCancelButtonClicked.Broadcast();
}