// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameChatWidget.generated.h"

class UEditableTextBox;
class UButton;
class UChatMessageWidget;
class IGameChatParentWidget;
class IChatMessageContainer;
class IChatMessageSender;

UCLASS()
class GAMECHAT_API UGameChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameChatWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Setup(IGameChatParentWidget* InParentWidget, IChatMessageContainer* MessageContainer);

	// Switches to typing message state
	void Open(IChatMessageSender* InOwnerMessageSender);

	// Switches back to overview state
	void Close();

	bool IsOpen() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* MessageContainerPanel;
	
	UPROPERTY(Meta = (BindWidget))
	UWidget* MessagePanel;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* MessageBox;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* SendMessageButton;
	
	TSubclassOf<UChatMessageWidget> ChatMessageClass;

	bool Opened;

	IGameChatParentWidget* ParentWidget;
	IChatMessageContainer* MessageContainer;
	IChatMessageSender* OwnerMessageSender;

	UFUNCTION()
	void RefreshMessages();

	UFUNCTION()
	void OnMessageBoxTextCommitted(const FText& MessageBoxText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void AttemptSendMessage();

};
