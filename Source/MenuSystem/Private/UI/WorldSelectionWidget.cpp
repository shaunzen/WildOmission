// Copyright Telephone Studios. All Rights Reserved.


#include "WorldSelectionWidget.h"
#include "UI/MainMenuWidget.h"
#include "WorldRowWidget.h"
#include "CreateWorldButtonWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"

UWorldSelectionWidget::UWorldSelectionWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	WorldListBox = nullptr;
	PlaySelectedWorldButton = nullptr;
	RenameWorldButton = nullptr;
	DeleteWorldButton = nullptr;
	CreateNewWorldButton = nullptr;
	MultiplayerButton = nullptr;
	CancelButton = nullptr;

	WorldRowWidgetClass = nullptr;

	static ConstructorHelpers::FClassFinder<UWorldRowWidget> WorldRowWidgetBPClass(TEXT("/Game/MenuSystem/UI/WBP_WorldRow"));
	if (WorldRowWidgetBPClass.Succeeded())
	{
		WorldRowWidgetClass = WorldRowWidgetBPClass.Class;
	}
}

void UWorldSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlaySelectedWorldButton->OnClicked.AddDynamic(this, &UWorldSelectionWidget::OnPlaySelectedWorldClicked);
	RenameWorldButton->OnClicked.AddDynamic(this, &UWorldSelectionWidget::OnRenameWorldClicked);
	DeleteWorldButton->OnClicked.AddDynamic(this, &UWorldSelectionWidget::OnDeleteWorldClicked);
	CreateNewWorldButton->OnClicked.AddDynamic(this, &UWorldSelectionWidget::OnCreateNewWorldClicked);
	MultiplayerButton->OnClicked.AddDynamic(this, &UWorldSelectionWidget::OnMultiplayerButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UWorldSelectionWidget::OnCancelButtonClicked);
}

void UWorldSelectionWidget::SetWorldList(const TArray<FString>& WorldNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	WorldListBox->ClearChildren();

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
		if (SaveGame->CreationInformation.Name == TEXT(""))
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

void UWorldSelectionWidget::OnPlaySelectedWorldClicked()
{
}

void UWorldSelectionWidget::OnRenameWorldClicked()
{
}

void UWorldSelectionWidget::OnDeleteWorldClicked()
{
}

void UWorldSelectionWidget::OnCreateNewWorldClicked()
{
}

void UWorldSelectionWidget::OnMultiplayerButtonClicked()
{
}

void UWorldSelectionWidget::OnCancelButtonClicked()
{
}
