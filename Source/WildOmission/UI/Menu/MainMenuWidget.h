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
	
	virtual void NativeConstruct() override;

	void Setup();
	void Teardown();

	void SetSaveList(TArray<FString> SaveNames);
	void SetServerList(TArray<FServerData> ServerNames);
	
	void SelectSaveIndex(uint32 Index);
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
	UButton* WorldSelectionPlayButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldSelectionCreateWorldButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldSelectionBrowseServersButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WorldSelectionBackButton;
	
	/*World Creation Menu*/

	/*Server Browser Menu*/

	/*Menus*/
	UPROPERTY(Meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
	UWidget* WorldSelectionMenu;

	UPROPERTY(Meta = (BindWidget))
	UWidget* WorldCreationMenu;

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
	void OpenServerBrowserMenu();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void PlaySelectedWorld();
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
