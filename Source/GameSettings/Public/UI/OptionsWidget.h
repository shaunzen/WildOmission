// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UMultiOptionBox;
class UButton;
class USliderOptionBox;

UCLASS()
class GAMESETTINGS_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetParent(UWidget* ParentMenu);

	void Refresh();

private:
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* FieldOfViewSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* MasterVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* WindowModeOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ResolutionOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* FrameRateLimitOptionBox;
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

	UPROPERTY(Meta = (BindWidget))
	UButton* ApplyButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* ResetButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY()
	UWidget* ParentMenu;

	void RefreshGameplaySettings();
	void RefreshWindowSettings();
	void RefreshGraphicsSettings();
	void RefreshCustomGraphicsSettings(bool IsUsingCustomSettings);

	UFUNCTION()
	void OnOverallQualityOptionChange(const FString& NewSelection);

	UFUNCTION()
	void Apply();

	void ApplyWindowSettings();
	void ApplyCustomGraphicsSettings();
	void ApplyFieldOfViewSettings();
	void ApplyMasterVolumeSettings();
	
	UFUNCTION()
	void Reset();

	UFUNCTION()
	void Back();

};
