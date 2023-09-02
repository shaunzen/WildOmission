// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameUserSettings.h"

static const float DEFAULT_FIELDOFVIEW = 90.0f;
static const float DEFAULT_MASTERVOLUME = 1.0f;

static const bool DEFAULT_AUTOEXPOSUREENABLED = true;
static const bool DEFAULT_MOTIONBLURENABLED = false;
static const bool DEFAULT_BLOOMENABLED = true;
static const bool DEFAULT_AMBIENTOCCLUSIONENABLED = true;
static const bool DEFAULT_FILMGRAINENABLED = false;

UWildOmissionGameUserSettings::UWildOmissionGameUserSettings(const FObjectInitializer& ObjectInitializer) : UGameUserSettings(ObjectInitializer)
{
    FieldOfView = DEFAULT_FIELDOFVIEW;
    MasterVolume = DEFAULT_MASTERVOLUME;

    AutoExposureEnabled = DEFAULT_AUTOEXPOSUREENABLED;
    MotionBlurEnabled = DEFAULT_MOTIONBLURENABLED;
    BloomEnabled = DEFAULT_BLOOMENABLED;
    AmbientOcclusionEnabled = DEFAULT_AMBIENTOCCLUSIONENABLED;
    FilmGrainEnabled = DEFAULT_FILMGRAINENABLED;

    HasRunAutoConfig = false;
}

void UWildOmissionGameUserSettings::SetToDefaults()
{
    Super::SetToDefaults();

    FieldOfView = DEFAULT_FIELDOFVIEW;
    MasterVolume = DEFAULT_MASTERVOLUME;

    AutoExposureEnabled = DEFAULT_AUTOEXPOSUREENABLED;
    MotionBlurEnabled = DEFAULT_MOTIONBLURENABLED;
    BloomEnabled = DEFAULT_BLOOMENABLED;
    AmbientOcclusionEnabled = DEFAULT_AMBIENTOCCLUSIONENABLED;
    FilmGrainEnabled = DEFAULT_FILMGRAINENABLED;
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

void UWildOmissionGameUserSettings::SetAutoExposureEnabled(bool UseAutoExposure)
{
    AutoExposureEnabled = UseAutoExposure;
}

bool UWildOmissionGameUserSettings::GetAutoExposureEnabled() const
{
    return AutoExposureEnabled;
}

void UWildOmissionGameUserSettings::SetMotionBlurEnabled(bool UseMotionBlur)
{
    MotionBlurEnabled = UseMotionBlur;
}

bool UWildOmissionGameUserSettings::GetMotionBlurEnabled() const
{
    return MotionBlurEnabled;
}

void UWildOmissionGameUserSettings::SetBloomEnabled(bool UseBloom)
{
    BloomEnabled = UseBloom;
}

bool UWildOmissionGameUserSettings::GetBloomEnabled() const
{
    return BloomEnabled;
}

void UWildOmissionGameUserSettings::SetAmbientOcclusionEnabled(bool UseAmbientOcclusion)
{
    AmbientOcclusionEnabled = UseAmbientOcclusion;
}

bool UWildOmissionGameUserSettings::GetAmbientOcclusionEnabled() const
{
    return AmbientOcclusionEnabled;
}

void UWildOmissionGameUserSettings::SetFilmGrainEnabled(bool UseFilmGrain)
{
    FilmGrainEnabled = UseFilmGrain;
}

bool UWildOmissionGameUserSettings::GetFilmGrainEnabled() const
{
    return FilmGrainEnabled;
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
