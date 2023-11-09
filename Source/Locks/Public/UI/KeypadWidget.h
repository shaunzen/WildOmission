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

	void Setup(class ALock* InLock, class ULockModifierComponent* InModifyingComponent, TEnumAsByte<ELockOperation> Operation);

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
	class UButton* BackspaceButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* LockActionButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* LockActionTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UWidget* UnlockSizeBox;
	UPROPERTY(Meta = (BindWidget))
	class UButton* UnlockButton;
	UPROPERTY(Meta = (BindWidget))
	class UWidget* RemoveLockSizeBox;
	UPROPERTY(Meta = (BindWidget))
	class UButton* RemoveLockButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	FString PendingCode;

	UFUNCTION()
	void OnOneClicked();
	UFUNCTION()
	void OnTwoClicked();
	UFUNCTION()
	void OnThreeClicked();
	UFUNCTION()
	void OnFourClicked();
	UFUNCTION()
	void OnFiveClicked();
	UFUNCTION()
	void OnSixClicked();
	UFUNCTION()
	void OnSevenClicked();
	UFUNCTION()
	void OnEightClicked();
	UFUNCTION()
	void OnNineClicked();
	UFUNCTION()
	void OnZeroClicked();
	UFUNCTION()
	void OnBackspaceClicked();

	UFUNCTION()
	void OnUnlockClicked();

	UFUNCTION()
	void OnRemoveLockClicked();

	void SetupStranger();
	void SetupAuthorized();

	UFUNCTION()
	void OnLockActionClicked();

	void AddCharacterToCode(const FString& CharacterToAdd);

	void RefreshCodeTextBlock();

	UPROPERTY()
	class ALock* LockToModify;

	UPROPERTY()
	class ULockModifierComponent* ModifyingComponent;

	UPROPERTY()
	TEnumAsByte<ELockOperation> LockOperation;

};
