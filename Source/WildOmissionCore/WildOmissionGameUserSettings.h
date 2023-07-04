// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "WildOmissionGameUserSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRenderDistanceChangedSignature, float, NewRenderDistance);

UCLASS()
class WILDOMISSIONCORE_API UWildOmissionGameUserSettings : public UGameUserSettings
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
	void SetRenderDistance(float NewRenderDistance);

	UFUNCTION()
	float GetRenderDistance() const;

	UFUNCTION()
	int32 GetResolutionScaleAsInt32() const;

	UFUNCTION()
	static UWildOmissionGameUserSettings* GetWildOmissionGameUserSettings();

	FOnRenderDistanceChangedSignature OnRenderDistanceChanged;

private:
	UPROPERTY(Config)
	float FieldOfView;
	
	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float RenderDistanceMeters;

};
