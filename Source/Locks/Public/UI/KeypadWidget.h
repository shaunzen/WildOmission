// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeypadWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class LOCKS_API UKeypadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UKeypadWidget(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CodeTextBlock;
	UPROPERTY(Meta = (BindWidget))
	UButton* OneKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* TwoKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ThreeKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* FourKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* FiveKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SixKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SevenKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* EightKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* NineKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ZeroKeyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ToggleLockButton;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ToggleLockTextBlock;
	UPROPERTY(Meta = (BindWidget))
	UButton* CloseButton;

};
