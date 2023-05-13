// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMenuWidget.generated.h"

class UTextBlock;
class UButton;
class UCheckBox;
class UEditableTextBox;
class UMainMenuWidget;

UCLASS()
class WILDOMISSION_API UWorldMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(UMainMenuWidget* InMainMenuParent);

	void Open(const FString& InWorldName);

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Title;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* PlayButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(Meta = (BindWidget))
	UCheckBox* MultiplayerCheckBox;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* HostSettingsMenu;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ServerNameInputBox;

	UPROPERTY()
	UMainMenuWidget* ParentMenu;

	FString WorldName;

	UFUNCTION()
	void ServerNameOnTextChanged(const FText& Text);
	
	UFUNCTION()
	void MultiplayerCheckboxChanged(bool bIsChecked);

	UFUNCTION()
	void PlayButtonClicked();

};
