// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeypadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeypadTeardownSignature);

UCLASS()
class LOCKS_API UKeypadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UKeypadWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Setup(class ALock* InLock);

	void Teardown();

	FOnKeypadTeardownSignature OnTeardown;

private:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CodeTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UButton* OneKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* TwoKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* ThreeKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* FourKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* FiveKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* SixKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* SevenKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* EightKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* NineKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* ZeroKeyButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* ToggleLockButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ToggleLockTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY()
	class ALock* LockToModify;

};
