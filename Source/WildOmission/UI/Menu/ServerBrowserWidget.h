// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerBrowserWidget.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

class UButton;
class UTextBlock;
class UServerRowWidget;

UCLASS()
class WILDOMISSION_API UServerBrowserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UServerBrowserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetServerList(TArray<FServerData> ServerNames);
	void SelectServerIndex(uint32 Index);

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* ServerList;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* JoinButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* RefreshListButton;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* RefreshListButtonText;

	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;

	TOptional<uint32> SelectedServerIndex;
	
	void UpdateServerListChildren();

	void RefreshList();

}; 