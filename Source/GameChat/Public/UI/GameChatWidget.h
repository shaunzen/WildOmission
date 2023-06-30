// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameChatWidget.generated.h"

class UEditableTextBox;
class UButton;
class UChatMessageWidget;

UCLASS()
class GAMECHAT_API UGameChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameChatWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Switches to typing message state
	void Open(UUserWidget* InParentHUD);
	// Switches back to overview state
	void Close();

	bool IsOpen() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* MessageContainer;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* MessagePanel;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* MessageBox;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* SendMessageButton;
	
	TSubclassOf<UChatMessageWidget> ChatMessageClass;

	bool Opened;

	UPlayerHUDWidget* ParentHUD;

	UFUNCTION()
	void RefreshMessages();

	UFUNCTION()
	void OnMessageBoxTextCommitted(const FText& MessageBoxText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void AttemptSendMessage();

};
