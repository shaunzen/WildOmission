// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "GameplaySettingsWidget.generated.h"

UCLASS()
class UGameplaySettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()

public:
	UGameplaySettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* ShowBrandingCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* ShowCrosshairCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* HideChatUnlessOpenCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* HideHUDCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UCheckOptionBox* CameraShakeEnabledCheckOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* FieldOfViewSliderOptionBox;
	
};
