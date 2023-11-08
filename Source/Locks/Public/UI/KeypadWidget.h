// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/LockOperation.h"
#include "KeypadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeypadTeardownSignature);

UCLASS()
class LOCKS_API UKeypadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UKeypadWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Setup(class ALock* InLock, TEnumAsByte<ELockOperation> Operation);

	UFUNCTION()
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
	class UButton* LockActionButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* LockActionTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	FString PendingCode;

	UFUNCTION()
	void OnOnePressed();
	UFUNCTION()
	void OnTwoPressed();
	UFUNCTION()
	void OnThreePressed();
	UFUNCTION()
	void OnFourPressed();
	UFUNCTION()
	void OnFivePressed();
	UFUNCTION()
	void OnSixPressed();
	UFUNCTION()
	void OnSevenPressed();
	UFUNCTION()
	void OnEightPressed();
	UFUNCTION()
	void OnNinePressed();
	UFUNCTION()
	void OnZeroPressed();

	UFUNCTION()
	void OnLockActionPressed();

	void AddCharacterToCode(const FString& CharacterToAdd);

	void RefreshCodeTextBlock();

	UPROPERTY()
	class ALock* LockToModify;

};
