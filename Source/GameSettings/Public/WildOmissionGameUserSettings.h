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
	bool HasRunAutoConfig;

};
