// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSignWidgetTeardownSignature);

UCLASS()
class DEPLOYABLES_API USignWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USignWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FOnSignWidgetTeardownSignature OnTeardown;

	UFUNCTION()
	void Setup(class ASign* InSign);

	UFUNCTION()
	void Teardown();

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
private:
	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* LineOneInputBox;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* LineTwoInputBox;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* LineThreeInputBox;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* LineFourInputBox;

	UPROPERTY(Meta = (BindWidget))
	class UButton* DoneButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY()
	class ASign* Sign;

	UFUNCTION()
	void OnLineOneInputBoxTextChanged(const FText& Text);

	UFUNCTION()
	void OnLineTwoInputBoxTextChanged(const FText& Text);

	UFUNCTION()
	void OnLineThreeInputBoxTextChanged(const FText& Text);

	UFUNCTION()
	void OnLineFourInputBoxTextChanged(const FText& Text);

	UFUNCTION()
	void PerformLineTrimOnInputBox(class UEditableTextBox* TextBox, const FText& Text);

	UFUNCTION()
	void OnDoneButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();

};
