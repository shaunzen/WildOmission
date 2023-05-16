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

	void RefreshMessages();

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* MessageContainer;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* MessageBox;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* SendMessageButton;
	
	TSubclassOf<UChatMessageWidget> ChatMessageClass;

	UFUNCTION()
	void SendMessage();

};
