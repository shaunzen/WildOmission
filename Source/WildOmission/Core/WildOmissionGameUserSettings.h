// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "WildOmissionGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UWildOmissionGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION()
	void SetFieldOfView(float NewFieldOfView);
	
	UFUNCTION()
	float GetFieldOfView() const;

	UFUNCTION()
	void SetMasterVolume(float NewMasterVolume);
	
	UFUNCTION()
	float GetMasterVolume() const;

	UFUNCTION()
	int32 GetResolutionScaleAsInt32() const;

	UFUNCTION()
	static UWildOmissionGameUserSettings* GetWildOmissionGameUserSettings();

private:
	UPROPERTY(Config)
	float FieldOfView;
	
	UPROPERTY(Config)
	float MasterVolume;

};
