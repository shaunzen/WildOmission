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
	UMultiOptionBox* ResolutionScaleOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* GraphicsQualityOptionBox;

	UPROPERTY(Meta = (BindWidget))
	UButton* ApplyButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY()
	UWidget* ParentMenu;

	void RefreshGameplaySettings();

	void RefreshGraphicsSettings();
	void RefreshGraphicsOptionBoxes(bool IsUsingCustomSettings);

	UFUNCTION()
	void CheckOverallScalabilityCustom(const FString& NewSelection);

	UFUNCTION()
	void Apply();
	void ApplyCustomGraphicsSettings();
	
	UFUNCTION()
	void Back();

};
