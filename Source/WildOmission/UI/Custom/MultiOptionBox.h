// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiOptionBox.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class WILDOMISSION_API UMultiOptionBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UMultiOptionBox(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	void AddOption(const FString& NewOption);
	void SetSelectedOption(const FString& NewOption);
	void SetSelectedIndex(const int32& NewIndex);
	void ClearOptions();

	FString GetSelectedOption() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* NextButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* PreviousButton;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SelectedTextBlock;
	
	TArray<FString> Options;
	int32 SelectedOptionIndex;

	UFUNCTION()
	void NextOption();
	
	UFUNCTION()
	void PreviousOption();

	void Refresh();

};
