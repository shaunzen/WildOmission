// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "AudioSettingsWidget.generated.h"

class USliderOptionBox;

UCLASS()
class UAudioSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* MasterVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* MusicVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* DeployablesVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* EnvironmentVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* FriendlyCreaturesVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* HostileCreaturesVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* PlayersVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	USliderOptionBox* WeatherVolumeSliderOptionBox;

};
