// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WildOmission/Core/Structs/ServerData.h"
#include "ServerBrowserWidget.generated.h"

class UButton;
class UTextBlock;
class UServerRowWidget;
class UMainMenuWidget;

UCLASS()
class WILDOMISSION_API UServerBrowserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UServerBrowserWidget(const FObjectInitializer& ObjectInitializer);

	void Setup(UMainMenuWidget* InMainMenuParent);

	void Open();

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

	UPROPERTY()
	UMainMenuWidget* ParentMenu;

	TSubclassOf<UServerRowWidget> ServerRowWidgetClass;

	TOptional<uint32> SelectedServerIndex;
	
	void UpdateServerListChildren();
	void JoinServer();
	void RefreshList();
}; 