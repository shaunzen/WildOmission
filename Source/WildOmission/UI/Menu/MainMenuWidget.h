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

class USaveRowWidget;
class UServerRowWidget;
class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UEditableTextBox;

UCLASS()
class WILDOMISSION_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	void Setup();
	void Teardown();

	void SetSaveList(TArray<FString> SaveNames);
	void SetServerList(TArray<FServerData> ServerNames);
	
	void SelectSaveIndex(uint32 Index);
	void SelectServerIndex(uint32 Index);
protected:
	virtual bool Initialize() override;

private:
	//*
	// Bind Widgets
	//*
	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	/*Main Menu*/
	UPROPERTY(Meta = (BindWidget))
	UButton* SingleplayerButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* MultiplayerButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ExitButton;

	/*Singleplayer Menu*/
	UPROPERTY(Meta = (BindWidget))
	UButton* SingleplayerSelectSaveButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SingleplayerNewSaveButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SingleplayerBackButton;
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* SaveList;

	/*Multiplayer Menu*/
	UPROPERTY(Meta = (BindWidget))
	UButton* MultiplayerJoinButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* MultiplayerHostButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* MultiplayerBackButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* RefreshServerListButton;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* RefreshServerListButtonText;
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* ServerList;
	
	/*New Save Menu*/
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* SaveNameInputBox;
	UPROPERTY(Meta = (BindWidget))
	UButton* NewSaveCreateButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* NewSaveBackButton;
	
	/*Host Menu*/
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ServerNameInputBox;
	UPROPERTY(Meta = (BindWidget))
	UButton* HostMenuHostButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* HostMenuBackButton;
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* HostSaveList;
	
	/*Menus*/
	UPROPERTY(Meta = (BindWidget))
	UWidget* MainMenu;
	UPROPERTY(Meta = (BindWidget))
	UWidget* SingleplayerMenu;
	UPROPERTY(Meta = (BindWidget))
	UWidget* MultiplayerMenu;
	UPROPERTY(Meta = (BindWidget))
	UWidget* NewSaveMenu;
	UPROPERTY(Meta = (BindWidget))
	UWidget* HostMenu;

	//*
	// Menu Functions
	//*
	UFUNCTION()
	void OpenMainMenu();
	UFUNCTION()
	void OpenMultiplayerMenu();
	UFUNCTION()
	void OpenSingleplayerMenu();
	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void OpenNewSaveMenu();
	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void LoadSave();
	UFUNCTION()
	void CreateSave();
	UFUNCTION()
	void JoinServer();
	UFUNCTION()
	void HostServer();
	
	UFUNCTION()
	void RefreshServerList();

	UFUNCTION()
	void SaveNameOnTextChanged(const FText& Text);
	UFUNCTION()
	void ServerNameOnTextChanged(const FText& Text);

	// TODO Save Row widget class
	TSubclassOf<USaveRowWidget> SaveRowWidgetClass;
	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;

	TOptional<uint32> SelectedSaveIndex;
	TOptional<uint32> SelectedServerIndex;
	
	void UpdateSaveListChildren();
	void UpdateServerListChildren();

};
