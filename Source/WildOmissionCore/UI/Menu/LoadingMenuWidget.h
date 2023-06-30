// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingMenuWidget.generated.h"

class UTextBlock;

UCLASS()
class WILDOMISSIONCORE_API ULoadingMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();
	void Teardown();

	void SetLoadingTitle(const FString& Title);
	void SetLoadingStep(const FString& StepName, const int32& StepPercent = -1);

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TitleTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SubtitleTextBlock;

};
