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

protected:
	virtual bool Initialize() override;

private:
	//*
	// Bind Widgets
	//*
	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	/*Main Menu*/

	/*Singleplayer Menu*/


	/*Multiplayer Menu*/

	
	/*New Save Menu*/

	
	/*Menus*/


	//*
	// Menu Functions
	//*
	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitGame();

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
