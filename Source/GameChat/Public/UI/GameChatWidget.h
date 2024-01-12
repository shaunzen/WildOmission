// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameChatWidget.generated.h"

class UEditableTextBox;
class UScrollBox;
class UChatMessageWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToggleRequestedSignature);

UCLASS()
class GAMECHAT_API UGameChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameChatWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Switches to typing message state
	void Open();

	// Switches back to overview state
	void Close();

	FOnToggleRequestedSignature OnToggleRequested;

	bool IsOpen() const;

	void SetHideUnlessOpen(bool InHideUnlessOpen);
	bool GetHideUnlessOpen() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UScrollBox* MessageContainerPanel;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* MessagePanel;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* MessageBox;
	
	TSubclassOf<UChatMessageWidget> ChatMessageClass;

	bool Opened;
	bool HideUnlessOpen;

	UFUNCTION()
	void RefreshMessages();

	UFUNCTION()
	void OnMessageBoxTextCommitted(const FText& MessageBoxText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void AttemptSendMessage();

};
