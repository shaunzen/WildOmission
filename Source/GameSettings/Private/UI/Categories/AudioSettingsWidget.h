// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SettingsCategoryWidget.h"
#include "AudioSettingsWidget.generated.h"

UCLASS()
class UAudioSettingsWidget : public USettingsCategoryWidget
{
	GENERATED_BODY()
	
public:
	UAudioSettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void OnApply() override;
	virtual void OnRefresh() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* MasterVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* MusicVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* DeployablesVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* EnvironmentVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* FriendlyCreaturesVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* HostileCreaturesVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* PlayersVolumeSliderOptionBox;
	UPROPERTY(Meta = (BindWidget))
	class USliderOptionBox* WeatherVolumeSliderOptionBox;

};
