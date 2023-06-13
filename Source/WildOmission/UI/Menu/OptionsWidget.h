// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UMultiOptionBox;
class UButton;
class USlider;

UCLASS()
class WILDOMISSION_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void Setup(UWidget* ParentMenu);

	void Refresh();

private:
	UPROPERTY(Meta = (BindWidget))
	USlider* FieldOfViewSlider;
	UPROPERTY(Meta = (BindWidget))
	USlider* MasterVolumeSlider;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* WindowModeOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* OverallGraphicsQualityOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ResolutionScaleOptionBox;
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
	UMultiOptionBox* ShaderQualityOptionBox;

	UPROPERTY(Meta = (BindWidget))
	UButton* ApplyButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY()
	UWidget* ParentMenu;

	void RefreshGameplaySettings();

	void RefreshGraphicsSettings();
	void RefreshCustomGraphicsOptionBoxes(bool IsUsingCustomSettings);

	UFUNCTION()
	void CheckOverallScalabilityCustom(const FString& NewSelection);

	UFUNCTION()
	void Apply();
	void ApplyCustomGraphicsSettings();
	
	UFUNCTION()
	void Back();

};
