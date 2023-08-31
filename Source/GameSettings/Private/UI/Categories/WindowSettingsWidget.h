// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "WindowSettingsWidget.generated.h"

class UMultiOptionBox;

UCLASS()
class UWindowSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* WindowModeOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* ResolutionOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UMultiOptionBox* FrameRateLimitOptionBox;

};
