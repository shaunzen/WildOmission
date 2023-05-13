// Copyright Telephone Studios. All Rights Reserved.


#include "WorldSelectionWidget.h"
#include "MainMenuWidget.h"
#include "WorldRowWidget.h"
#include "CreateWorldButtonWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"

UWorldSelectionWidget::UWorldSelectionWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UWorldRowWidget> WorldRowWidgetBPClass(TEXT("/Game/WildOmission/UI/Menu/WBP_WorldRow"));
	if (WorldRowWidgetBPClass.Succeeded())
	{
		WorldRowWidgetClass = WorldRowWidgetBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UCreateWorldButtonWidget> CreateNewWorldBPClass(TEXT("/Game/WildOmission/UI/Menu/WBP_CreateWorldButton"));
	if (CreateNewWorldBPClass.Succeeded())
	{
		CreateNewWorldButtonClass = CreateNewWorldBPClass.Class;
	}
}

void UWorldSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMainMenuWidget* ParentMenu = Cast<UMainMenuWidget>(GetParent());
	
	if (ParentMenu == nullptr)
	{
		return;
	}

	SelectButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldMenu);
	BrowseServersButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenServerBrowserMenu);
	BackButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenMainMenu);
}

void UWorldSelectionWidget::SetWorldList(const TArray<FString>& WorldNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	WorldListBox->ClearChildren();

	UCreateWorldButtonWidget* CreateWorldButton = CreateWidget<UCreateWorldButtonWidget>(World, CreateNewWorldButtonClass);
	if (CreateWorldButton == nullptr)
	{
		return;
	}

	UMainMenuWidget* ParentMenu = Cast<UMainMenuWidget>(GetParent());
	
	if (ParentMenu == nullptr)
	{
		return;
	}

	CreateWorldButton->GetButton()->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldCreationMenu);

	WorldListBox->AddChild(CreateWorldButton);

	TArray<UWildOmissionSaveGame*> SortedWorlds = GetWorldsSortedByLastPlayed(WorldNames);

	for (UWildOmissionSaveGame* SaveGame : SortedWorlds)
	{
		if (SaveGame == nullptr)
		{
			continue;
		}

		UWorldRowWidget* Row = CreateWidget<UWorldRowWidget>(World, WorldRowWidgetClass);
		if (Row == nullptr || SaveGame == nullptr)
		{
			return;
		}

		FString DaysPlayedString = FString::Printf(TEXT("%i Days"), SaveGame->DaysPlayed);
		FString CreationString = FString::Printf(TEXT("Created: %i/%i/%i"), SaveGame->CreationInformation.Month, SaveGame->CreationInformation.Day, SaveGame->CreationInformation.Year);

		Row->WorldNameTextBlock->SetText(FText::FromString(SaveGame->CreationInformation.Name));
		Row->DaysPlayed->SetText(FText::FromString(DaysPlayedString));
		Row->DateCreated->SetText(FText::FromString(CreationString));
		Row->Setup(this, SaveGame->CreationInformation.Name);

		WorldListBox->AddChild(Row);
	}
}

void UWorldSelectionWidget::SetSelectedWorld(const FString& WorldName)
{
	SelectedWorldName = WorldName;
	UpdateListChildren();
}

void UWorldSelectionWidget::UpdateListChildren()
{
	for (int32 i = 0; i < WorldListBox->GetChildrenCount(); ++i)
	{
		UWorldRowWidget* Row = Cast<UWorldRowWidget>(WorldListBox->GetChildAt(i));

		if (Row == nullptr)
		{
			continue;
		}

		bool RowSelected = (SelectedWorldName.IsSet() && SelectedWorldName.GetValue() == Row->GetWorldName());
		
		Row->Selected = RowSelected;
	}
}

TArray<UWildOmissionSaveGame*> UWorldSelectionWidget::GetWorldsSortedByLastPlayed(const TArray<FString>& NamesList)
{
	TArray<UWildOmissionSaveGame*> SortedSaveGames;
	for (const FString& WorldName : NamesList)
	{
		UWildOmissionSaveGame* SaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(WorldName, 0));
		if (SaveGame == nullptr)
		{
			continue;
		}

		// If this save was created before pre alpha 0.7.3, populate its Name Value
		if (SaveGame->CreationInformation.Name == FString())
		{
			SaveGame->CreationInformation.Name = WorldName;
			UGameplayStatics::SaveGameToSlot(SaveGame, WorldName, 0);
		}

		SortedSaveGames.Add(SaveGame);
	}

	Algo::Sort(SortedSaveGames, IsSaveMoreRecentlyPlayed);

	return SortedSaveGames;
}

bool UWorldSelectionWidget::IsSaveMoreRecentlyPlayed(UWildOmissionSaveGame* SaveA, UWildOmissionSaveGame* SaveB)
{
	return SaveA->LastPlayedTime > SaveB->LastPlayedTime;
}
