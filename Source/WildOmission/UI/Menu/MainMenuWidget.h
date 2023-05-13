// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


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

};
