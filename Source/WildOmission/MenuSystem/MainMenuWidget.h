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
	
	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

private:
	//*
	// Bind Widgets
	//*
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmJoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CancelHostMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmHostMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;
	
	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;
	
	//*
	// Menu Functions
	//*
	UFUNCTION()
	void HostServer();
	
	UFUNCTION()
	void JoinServer();
	
	UFUNCTION()
	void OpenHostMenu();
	
	UFUNCTION()
	void OpenJoinMenu();
	
	UFUNCTION()
	void OpenMainMenu();
	
	UFUNCTION()
	void QuitPressed();
	
	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;
	TOptional<uint32> SelectedIndex;
	void UpdateChildren();

};
