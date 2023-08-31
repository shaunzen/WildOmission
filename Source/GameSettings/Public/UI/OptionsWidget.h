// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UMultiOptionBox;
class UButton;
class USliderOptionBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsOnBackButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFieldOfViewChangedSignature);

UCLASS()
class GAMESETTINGS_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void Refresh();

	FOptionsOnBackButtonPressedSignature OnBackButtonPressed;
	FOnFieldOfViewChangedSignature OnFieldOfViewChanged;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* ApplyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ResetButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void Apply();

	UFUNCTION()
	void Reset();

	UFUNCTION()
	void Back();

};
