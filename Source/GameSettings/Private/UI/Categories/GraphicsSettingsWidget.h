// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "GraphicsSettingsWidget.generated.h"

class UMultiOptionBox;

UCLASS()
class UGraphicsSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* OverallGraphicsQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ViewDistanceQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ShadowQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* GlobalIlluminationQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ReflectionQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* AntiAliasingQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* TextureQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* VisualEffectQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* PostProcessingQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* FoliageQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ShaderQualityOptionBox;
	
	void RefreshCustomGraphicsSettings(bool IsUsingCustomSettings);

	UFUNCTION()
	void OnOverallQualityOptionChange(const FString& NewSelection);

};
