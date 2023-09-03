// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "ControlsSettingsWidget.generated.h"

class UVerticalBox;
class UKeyOptionBox;

UCLASS()
class UControlsSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* MoveForwardKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* MoveBackwardKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* MoveLeftKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* MoveRightKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* SprintKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* JumpKeyOptionBox;

	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* PrimaryKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* SecondaryKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* InteractKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* ReloadKeyOptionBox;

	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* InventoryKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* CraftingKeyOptionBox;
	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* ChatKeyOptionBox;


};
