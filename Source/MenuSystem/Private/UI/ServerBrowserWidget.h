// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ServerData.h"
#include "ServerBrowserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerBrowserOnJoinButtonClickedSignature, const uint32&, SelectedIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServerBrowserOnRefreshButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServerBrowserOnCancelButtonClickedSignature);

UCLASS()
class UServerBrowserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UServerBrowserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FServerBrowserOnJoinButtonClickedSignature OnJoinButtonClicked;
	FServerBrowserOnRefreshButtonClickedSignature OnRefreshButtonClicked;
	FServerBrowserOnCancelButtonClickedSignature OnCancelButtonClicked;

	void OnOpen();

	void SetServerList(TArray<struct FServerData> ServerNames);
	
private:
	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* ServerList;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* RefreshListButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* RefreshListButtonText;

	TSubclassOf<class UServerRowWidget> ServerRowWidgetClass;

	TOptional<uint32> SelectedServerIndex;
	
	UFUNCTION()
	void UpdateServerListChildren();
	
	UFUNCTION()
	void SelectServerIndex(const uint32& SelectedIndex);

	UFUNCTION()
	void BroadcastRefreshButtonClicked();

	UFUNCTION()
	void BroadcastJoinButtonClicked();

	UFUNCTION()
	void BroadcastCancelButtonClicked();

}; 