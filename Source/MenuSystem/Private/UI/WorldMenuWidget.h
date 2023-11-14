// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FWorldMenuOnPlayButtonClickedSignature, const FString&, WorldName, const FString&, ServerName, const bool, IsMultiplayer, const bool, IsFriendsOnly);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWorldMenuOnBackButtonClickedSignature);

UCLASS()
class UWorldMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWorldMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Open(const FString& InWorldName);

	FWorldMenuOnPlayButtonClickedSignature OnPlayButtonClicked;
	FWorldMenuOnBackButtonClickedSignature OnBackButtonClicked;

private:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* Title;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* PlayButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* MultiplayerCheckOptionBox;
	
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* FriendsOnlyCheckOptionBox;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* HostSettingsMenu;
	
	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* ServerNameInputBox;

	FString WorldName;

	UFUNCTION()
	void ServerNameOnTextChanged(const FText& Text);
	
	UFUNCTION()
	void MultiplayerCheckboxChanged(bool bIsChecked);

	UFUNCTION()
	void BroadcastPlayButtonClicked();

	UFUNCTION()
	void BroadcastBackButtonClicked();

};
