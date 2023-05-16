// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameChatWidget.generated.h"

class UEditableTextBox;
class UButton;
class UChatMessageWidget;

UCLASS()
class WILDOMISSION_API UGameChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameChatWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Switches to typing message state
	void Open();
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

	UFUNCTION()
	void SendMessage();

	UFUNCTION()
	void RefreshMessages();

};
