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
	
	//*
	// Menu Functions
	//*
	
	// TODO Save Row widget class
	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;
	TOptional<uint32> SelectedSaveIndex;
	TOptional<uint32> SelectedServerIndex;
	void UpdateServerListChildren();

};
