// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "PostProcessingSettingsWidget.generated.h"

class UCheckOptionBox;

UCLASS()
class UPostProcessingSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* AutoExposureCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* MotionBlurCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* BloomCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* AmbientOcclusionCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* FilmGrainCheckOptionBox;
	
};
