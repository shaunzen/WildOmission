// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "PostProcessingSettingsWidget.generated.h"

UCLASS()
class UPostProcessingSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()

public:
	UPostProcessingSettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* GammaSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* AutoExposureCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* MotionBlurCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* BloomCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* AmbientOcclusionCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* FilmGrainCheckOptionBox;
	
};
