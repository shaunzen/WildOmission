// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

class UTextBlock;
class UGameChatWidget;

UCLASS()
class UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UGameChatWidget* InParent, const FString& PlayerName, const FString& Message, const double& InTimeSent);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* PlayerNameText;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* MessageText;
	
	double TimeSent;
	
	UPROPERTY()
	UGameChatWidget* ParentChatWidget;

};
