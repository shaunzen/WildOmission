// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UMultiOptionBox;
class UButton;
class USlider;

UENUM()
enum EResolutionScale
{
	SCALE_20, SCALE_30, SCALE_40, SCALE_50, SCALE_60, SCALE_70, SCALE_80, SCALE_90, SCALE_100, SCALE_110, SCALE_120, SCALE_130, SCALE_140, SCALE_150
};

UCLASS()
class WILDOMISSION_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void Setup(UWidget* ParentMenu);

	void Open();

private:
	UPROPERTY(Meta = (BindWidget))
	USlider* FOVSlider;
	UPROPERTY(Meta = (BindWidget))
	USlider* VolumeSlider;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* WindowModeOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ResolutionScaleOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* GraphicsQualityOptionBox;

	UPROPERTY(Meta = (BindWidget))
	UButton* ApplyButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY()
	UWidget* ParentMenu;

	UFUNCTION()
	void Apply();
	
	UFUNCTION()
	void Back();

};
