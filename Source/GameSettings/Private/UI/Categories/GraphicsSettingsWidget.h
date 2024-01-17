// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "GraphicsSettingsWidget.generated.h"

UCLASS()
class UGraphicsSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
	
public:
	UGraphicsSettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* RunHardwareBenchmarkButton;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* OverallGraphicsQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* ViewDistanceQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* ShadowQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* GlobalIlluminationQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* ReflectionQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* AntiAliasingQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* TextureQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* VisualEffectQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* PostProcessingQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* FoliageQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* ShaderQualityOptionBox;
	
	void RefreshCustomGraphicsSettings(bool IsUsingCustomSettings);

	UFUNCTION()
	void OnOverallQualityOptionChange(const FString& NewSelection);

	UFUNCTION()
	void RunHardwareBenchmark();

};
