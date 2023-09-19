// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Categories/SettingsCategoryWidget.h"
#include "AudioSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class UAudioSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:

};
