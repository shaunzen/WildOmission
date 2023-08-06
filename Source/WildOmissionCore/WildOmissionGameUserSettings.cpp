// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameUserSettings.h"

static const float DEFAULT_FIELDOFVIEW = 90.0f;
static const float DEFAULT_MASTERVOLUME = 1.0f;

UWildOmissionGameUserSettings::UWildOmissionGameUserSettings(const FObjectInitializer& ObjectInitializer) : UGameUserSettings(ObjectInitializer)
{
    FieldOfView = DEFAULT_FIELDOFVIEW;
    MasterVolume = DEFAULT_MASTERVOLUME;
    HasRunAutoConfig = false;
}

void UWildOmissionGameUserSettings::SetToDefaults()
{
    Super::SetToDefaults();

    FieldOfView = DEFAULT_FIELDOFVIEW;
    MasterVolume = DEFAULT_MASTERVOLUME;
}

void UWildOmissionGameUserSettings::SetFieldOfView(float NewFieldOfView)
{
    FieldOfView = NewFieldOfView;
}

float UWildOmissionGameUserSettings::GetFieldOfView() const
{
    return FieldOfView;
}

void UWildOmissionGameUserSettings::SetMasterVolume(float NewMasterVolume)
{
    MasterVolume = NewMasterVolume;
}

float UWildOmissionGameUserSettings::GetMasterVolume() const
{
    return MasterVolume;
}

void UWildOmissionGameUserSettings::SetHasRunAutoConfig(bool HasRun)
{
    HasRunAutoConfig = HasRun;
}

bool UWildOmissionGameUserSettings::GetHasRunAutoConfig() const
{
    return HasRunAutoConfig;
}

int32 UWildOmissionGameUserSettings::GetResolutionScaleAsInt32() const
{
    float CurrentScaleNormalized = 0.0f;
    float CurrentScale = 0.0f;
    float MinScale = 0.0f;
    float MaxScale = 0.0f;
    GetResolutionScaleInformationEx(CurrentScaleNormalized, CurrentScale, MinScale, MaxScale);
    return FMath::RoundToInt32(CurrentScale);
}

UWildOmissionGameUserSettings* UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings()
{
    return Cast<UWildOmissionGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}
