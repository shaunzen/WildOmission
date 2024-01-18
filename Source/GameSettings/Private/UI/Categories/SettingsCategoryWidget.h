// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsCategoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class USettingsCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnApply();
	UFUNCTION()
	virtual void OnReset();
	UFUNCTION()
	virtual void OnRefresh();

};
