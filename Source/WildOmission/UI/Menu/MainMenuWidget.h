// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

class UWorldRowWidget;
class UServerRowWidget;
class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UEditableTextBox;
class UCheckBox;
class UCreateWorldButtonWidget;
class UWildOmissionSaveGame;

UCLASS()
class WILDOMISSION_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	void Setup();
	void Teardown();

	void SetSaveList(TArray<FString> SaveNames);
	void SetServerList(TArray<FServerData> ServerNames);
	
	void SetSelectedWorld(const FString& WorldName);
	void SelectServerIndex(uint32 Index);

private:
	//*
	// Bind Widgets
	//*
	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	/*Main Menu*/
	UPROPERTY(Meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ExitButton;

	/*World Selection Menu*/
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* WorldListBox;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldSelectionSelectButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldSelectionBrowseServersButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldSelectionBackButton;

	/*World Creation Menu*/
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldCreationCreateWorldButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldCreationBackButton;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* WorldNameInputBox;

	/*World Menu*/
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* WorldMenuTitle;
	UPROPERTY(Meta = (BindWidget))
	UButton* PlayWorldButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldBackButton;
	UPROPERTY(Meta = (BindWidget))
	UCheckBox* MultiplayerCheckBox;
	UPROPERTY(Meta = (BindWidget))
	UWidget* HostSettingsMenu;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ServerNameInputBox;

	/*Server Browser Menu*/
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* ServerList;
	UPROPERTY(Meta = (BindWidget))
	UButton* ServerJoinButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ServerBackButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* RefreshServerListButton;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* RefreshServerListButtonText;
	
	/*Menus*/
	UPROPERTY(Meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
	UWidget* WorldSelectionMenu;

	UPROPERTY(Meta = (BindWidget))
	UWidget* WorldCreationMenu;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* WorldMenu;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* ServerBrowserMenu;
	
	//*
	// Menu Functions
	//*
	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenWorldSelectionMenu();

	UFUNCTION()
	void OpenWorldCreationMenu();

	UFUNCTION()
	void OpenWorldMenu();

	UFUNCTION()
	void OpenServerBrowserMenu();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void PlaySelectedWorld();
	UFUNCTION()
	void CreateWorld();
	UFUNCTION()
	void JoinServer();
	
	UFUNCTION()
	void RefreshServerList();

	UFUNCTION()
	void WorldNameOnTextChanged(const FText& Text);
	UFUNCTION()
	void ServerNameOnTextChanged(const FText& Text);
	UFUNCTION()
	void MultiplayerCheckboxChanged(bool bIsChecked);

	// TODO Make single widget class
	TSubclassOf<UWorldRowWidget> WorldRowWidgetClass;
	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;
	TSubclassOf<UCreateWorldButtonWidget> CreateNewWorldButtonClass;

	TOptional<FString> SelectedWorldName;
	TOptional<uint32> SelectedServerIndex;
	
	void UpdateSaveListChildren();
	void UpdateServerListChildren();

	TArray<UWildOmissionSaveGame*> GetWorldsSortedByLastPlayed(const TArray<FString>& NameList);
	static bool IsSaveMoreRecentlyPlayed(UWildOmissionSaveGame* SaveA, UWildOmissionSaveGame* SaveB);
};
