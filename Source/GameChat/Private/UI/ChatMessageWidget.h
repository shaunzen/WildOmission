// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

UCLASS()
class UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(class UGameChatWidget* InParent, const struct FChatMessage& InChatMessage);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* PlayerNameText;
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* MessageText;
	
	double TimeRecieved;
	
	UPROPERTY()
	class UGameChatWidget* ParentChatWidget;

};
