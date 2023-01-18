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

class UServerRowWidget;
class UButton;
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

	void SetServerList(TArray<FServerData> ServerNames);
	
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

	/*Multiplayer Menu*/
	// TODO

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
	void OpenSingleplayerMenu();
	UFUNCTION()
	void OpenMultiplayerMenu();
	UFUNCTION()
	void ExitGame();
	UFUNCTION()
	void LoadSave();
	UFUNCTION()
	void OpenNewSaveMenu();

	// TODO Save Row widget class
	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;
	TOptional<uint32> SelectedSaveIndex;
	TOptional<uint32> SelectedServerIndex;
	void UpdateServerListChildren();

};
