// Copyright Telephone Studios. All Rights Reserved.


#include "UI/Custom/SliderOptionBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void USliderOptionBox::NativeConstruct()
{
	Slider->OnValueChanged.AddDynamic(this, &USliderOptionBox::OnSliderValueChanged);
}

void USliderOptionBox::SetValue(float Value)
{
	Slider->SetValue(Value);
	OnSliderValueChanged(Value);
}

void USliderOptionBox::SetMinValue(float Value)
{
	Slider->SetMinValue(Value);
	OnSliderValueChanged(Value);
}

void USliderOptionBox::SetMaxValue(float Value)
{
	Slider->SetMaxValue(Value);
	OnSliderValueChanged(Value);
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
