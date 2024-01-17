// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "WindowSettingsWidget.generated.h"

UCLASS()
class UWindowSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()

public:
	UWindowSettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* WindowModeOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* ResolutionOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class UMultiOptionBox* FrameRateLimitOptionBox;

};
