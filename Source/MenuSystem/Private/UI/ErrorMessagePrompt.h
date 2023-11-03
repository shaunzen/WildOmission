// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorMessagePrompt.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseButtonClickedSignature);

UCLASS()
class UErrorMessagePrompt : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UErrorMessagePrompt(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetMessage(const FString& MessageTitle, const FString& Message);

	FOnCloseButtonClickedSignature OnCloseButtonClicked;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TitleTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* MessageTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UButton* CloseButton;

	UFUNCTION()
	void BroadcastCloseButtonClicked();

};
