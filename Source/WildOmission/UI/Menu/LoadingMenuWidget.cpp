// Copyright Telephone Studios. All Rights Reserved.


#include "LoadingMenuWidget.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"

void ULoadingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void ULoadingMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

void ULoadingMenuWidget::SetLoadingTitle(const FString& Title)
{
	TitleTextBlock->SetText(FText::FromString(Title));
}

void ULoadingMenuWidget::SetLoadingStep(const FString& StepName, const int32& StepPercent)
{
	FString StepText = FString();
	
	if (StepPercent == -1)
	{
		StepText = StepName;
	}
	else
	{
		StepText = FString::Printf(TEXT("%s: %i%"), *StepName, StepPercent);
	}

	SubtitleTextBlock->SetText(FText::FromString(StepText));
}