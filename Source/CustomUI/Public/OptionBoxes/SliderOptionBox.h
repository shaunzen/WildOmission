// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderOptionBox.generated.h"

class USlider;
class UTextBlock;

UCLASS()
class CUSTOMUI_API USliderOptionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	USliderOptionBox(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

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
	USlider* Slider;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	bool ShowDecimal;
	bool RoundAfterValueChanged;

	UFUNCTION()
	void OnSliderValueChanged(float Value);

};
