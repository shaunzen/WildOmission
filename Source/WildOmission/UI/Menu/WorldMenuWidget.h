// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMenuWidget.generated.h"

class UTextBlock;
class UButton;
class UCheckBox;
class UEditableTextBox;

UCLASS()
class WILDOMISSION_API UWorldMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
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

	FString WorldName;

	UFUNCTION()
	void ServerNameOnTextChanged(const FText& Text);
	
	UFUNCTION()
	void MultiplayerCheckboxChanged(bool bIsChecked);

	UFUNCTION()
	void PlayButtonClicked();

};
