// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

class UTextBlock;

UCLASS()
class WILDOMISSION_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(const FString& PlayerName, const FString& Message, const int32& Index);

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* PlayerNameText;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* MessageText;
	
	int32 MessageIndex;

};
