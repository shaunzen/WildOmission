// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "WildOmissionGameUserSettings.generated.h"

UCLASS()
class GAMESETTINGS_API UWildOmissionGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:
	virtual void SetToDefaults() override;

	UFUNCTION()
	void SetFieldOfView(float NewFieldOfView);
	
	UFUNCTION()
	float GetFieldOfView() const;

	UFUNCTION()
	void SetMasterVolume(float NewMasterVolume);
	
	UFUNCTION()
	float GetMasterVolume() const;

	UFUNCTION()
	void SetAutoExposureEnabled(bool UseAutoExposure);

	UFUNCTION()
	bool GetAutoExposureEnabled() const;

	UFUNCTION()
	void SetMotionBlurEnabled(bool UseMotionBlur);

	UFUNCTION()
	bool GetMotionBlurEnabled() const;

	UFUNCTION()
	void SetBloomEnabled(bool UseBloom);

	UFUNCTION()
	bool GetBloomEnabled() const;

	UFUNCTION()
	void SetAmbientOcclusionEnabled(bool UseAmbientOcclusion);

	UFUNCTION()
	bool GetAmbientOcclusionEnabled() const;

	UFUNCTION()
	void SetFilmGrainEnabled(bool UseFilmGrain);

	UFUNCTION()
	bool GetFilmGrainEnabled() const;

	UFUNCTION()
	void SetHasRunAutoConfig(bool HasRun);

	UFUNCTION()
	bool GetHasRunAutoConfig() const;

	UFUNCTION()
	int32 GetResolutionScaleAsInt32() const;

	UFUNCTION()
	static UWildOmissionGameUserSettings* GetWildOmissionGameUserSettings();

private:
	UPROPERTY(Config)
	float FieldOfView;
	
	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	bool AutoExposureEnabled;

	UPROPERTY(Config)
	bool MotionBlurEnabled;

	UPROPERTY(Config)
	bool BloomEnabled;

	UPROPERTY(Config)
	bool AmbientOcclusionEnabled;

	UPROPERTY(Config)
	bool FilmGrainEnabled;

	UPROPERTY(Config)
	bool HasRunAutoConfig;

};
