// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsCategoryMenu.generated.h"

/**
 * 
 */
UCLASS()
class USettingsCategoryMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnApply();
	virtual void OnReset();
	
};
