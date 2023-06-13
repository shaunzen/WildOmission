// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderOptionBox.generated.h"

class USlider;
class UTextBlock;

UCLASS()
class WILDOMISSION_API USliderOptionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(float Default, float Min, float Max);

	void SetValue(float Value);
	float GetValue() const;

private:
	UPROPERTY(Meta = (BindWidget))
	USlider* Slider;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	UFUNCTION()
	void OnSliderValueChanged(float Value);

};
