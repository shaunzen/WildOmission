// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderOptionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderOptionBoxValueChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSliderOptionBoxValueChangedNoParamsSignature);

UCLASS()
class CUSTOMUI_API USliderOptionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	USliderOptionBox(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FOnSliderOptionBoxValueChangedSignature OnValueChanged;
	FOnSliderOptionBoxValueChangedNoParamsSignature OnValueChangedNoParams;

	void SetValue(float Value);
	void SetMinValue(float Value);
	void SetMaxValue(float Value);
	float GetValue() const;

	void SetShowDecimal(bool Show);
	bool GetShowDecimal() const;
	
	void SetRoundAfterValueChanged(bool Round);
	bool GetRoundAfterValueChanged() const;

private:
	UPROPERTY(Meta = (BindWidget))
	class USlider* Slider;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock;

	bool ShowDecimal;
	bool RoundAfterValueChanged;

	UFUNCTION()
	void OnSliderValueChanged(float Value);

};
