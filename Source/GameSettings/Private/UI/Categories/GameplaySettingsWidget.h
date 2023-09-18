// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "GameplaySettingsWidget.generated.h"

class UCheckOptionBox;
class USliderOptionBox;

UCLASS()
class UGameplaySettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* ShowBrandingCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* ShowCrosshairCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UCheckOptionBox* CameraShakeEnabledCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* FieldOfViewSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* MasterVolumeSliderOptionBox;
	
};
