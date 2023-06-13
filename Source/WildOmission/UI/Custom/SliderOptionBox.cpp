// Copyright Telephone Studios. All Rights Reserved.


#include "SliderOptionBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void USliderOptionBox::Setup(float Default, float Min, float Max)
{
	Slider->SetMinValue(Min);
	Slider->SetMaxValue(Max);
	Slider->SetValue(Default);
	Slider->OnValueChanged.AddDynamic(this, &USliderOptionBox::OnSliderValueChanged);
}

void USliderOptionBox::SetValue(float Value)
{
	Slider->SetValue(Value);
}

float USliderOptionBox::GetValue() const
{
	return Slider->GetValue();
}

void USliderOptionBox::OnSliderValueChanged(float Value)
{
	FString ValueString = FString::Printf(TEXT("%i"), FMath::RoundToInt32(Value));
	TextBlock->SetText(FText::FromString(ValueString));
}
