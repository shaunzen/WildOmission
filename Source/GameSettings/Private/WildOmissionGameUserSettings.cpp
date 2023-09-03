// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameUserSettings.h"

static const float DEFAULT_FIELDOFVIEW = 90.0f;
static const float DEFAULT_MASTERVOLUME = 1.0f;

static const bool DEFAULT_AUTOEXPOSUREENABLED = true;
static const bool DEFAULT_MOTIONBLURENABLED = false;
static const bool DEFAULT_BLOOMENABLED = true;
static const bool DEFAULT_AMBIENTOCCLUSIONENABLED = true;
static const bool DEFAULT_FILMGRAINENABLED = false;

static const FKey DEFAULT_MOVEFORWARD = EKeys::W;
static const FKey DEFAULT_MOVEBACKWARD = EKeys::S;
static const FKey DEFAULT_MOVELEFT = EKeys::A;
static const FKey DEFAULT_MOVERIGHT = EKeys::D;
static const FKey DEFAULT_SPRINT = EKeys::LeftShift;
static const FKey DEFAULT_JUMP = EKeys::SpaceBar;

static const FKey DEFAULT_PRIMARY = EKeys::LeftMouseButton;
static const FKey DEFAULT_SECONDARY = EKeys::RightMouseButton;
static const FKey DEFAULT_INTERACT = EKeys::E;
static const FKey DEFAULT_RELOAD = EKeys::R;

static const FKey DEFAULT_INVENTORY = EKeys::Tab;
static const FKey DEFAULT_CRAFTING = EKeys::Q;
static const FKey DEFAULT_CHAT = EKeys::T;


UWildOmissionGameUserSettings::UWildOmissionGameUserSettings(const FObjectInitializer& ObjectInitializer) : UGameUserSettings(ObjectInitializer)
{
    FieldOfView = DEFAULT_FIELDOFVIEW;
    MasterVolume = DEFAULT_MASTERVOLUME;

    AutoExposureEnabled = DEFAULT_AUTOEXPOSUREENABLED;
    MotionBlurEnabled = DEFAULT_MOTIONBLURENABLED;
    BloomEnabled = DEFAULT_BLOOMENABLED;
    AmbientOcclusionEnabled = DEFAULT_AMBIENTOCCLUSIONENABLED;
    FilmGrainEnabled = DEFAULT_FILMGRAINENABLED;

    MoveForwardKey = DEFAULT_MOVEFORWARD;
    MoveBackwardKey = DEFAULT_MOVEBACKWARD;
    MoveLeftKey = DEFAULT_MOVELEFT;
    MoveRightKey = DEFAULT_MOVERIGHT;
    SprintKey = DEFAULT_SPRINT;
    JumpKey = DEFAULT_JUMP;

    PrimaryKey = DEFAULT_PRIMARY;
    SecondaryKey = DEFAULT_SECONDARY;
    InteractKey = DEFAULT_INTERACT;
    ReloadKey = DEFAULT_RELOAD;

    InventoryKey = DEFAULT_INVENTORY;
    CraftingKey = DEFAULT_CRAFTING;
    ChatKey = DEFAULT_CHAT;

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

    MoveForwardKey = DEFAULT_MOVEFORWARD;
    MoveBackwardKey = DEFAULT_MOVEBACKWARD;
    MoveLeftKey = DEFAULT_MOVELEFT;
    MoveRightKey = DEFAULT_MOVERIGHT;
    SprintKey = DEFAULT_SPRINT;
    JumpKey = DEFAULT_JUMP;

    PrimaryKey = DEFAULT_PRIMARY;
    SecondaryKey = DEFAULT_SECONDARY;
    InteractKey = DEFAULT_INTERACT;
    ReloadKey = DEFAULT_RELOAD;

    InventoryKey = DEFAULT_INVENTORY;
    CraftingKey = DEFAULT_CRAFTING;
    ChatKey = DEFAULT_CHAT;
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

void UWildOmissionGameUserSettings::SetMoveForwardKey(const FKey& NewKey)
{
    MoveForwardKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetMoveForwardKey() const
{
    return MoveForwardKey;
}

void UWildOmissionGameUserSettings::SetMoveBackwardKey(const FKey& NewKey)
{
    MoveBackwardKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetMoveBackwardKey() const
{
    return MoveBackwardKey;
}

void UWildOmissionGameUserSettings::SetMoveLeftKey(const FKey& NewKey)
{
    MoveLeftKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetMoveLeftKey() const
{
    return MoveLeftKey;
}

void UWildOmissionGameUserSettings::SetMoveRightKey(const FKey& NewKey)
{
    MoveRightKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetMoveRightKey() const
{
    return MoveRightKey;
}

void UWildOmissionGameUserSettings::SetSprintKey(const FKey& NewKey)
{
    SprintKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetSprintKey() const
{
    return SprintKey;
}

void UWildOmissionGameUserSettings::SetJumpKey(const FKey& NewKey)
{
    JumpKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetJumpKey() const
{
    return JumpKey;
}

void UWildOmissionGameUserSettings::SetPrimaryKey(const FKey& NewKey)
{
    PrimaryKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetPrimaryKey() const
{
    return PrimaryKey;
}

void UWildOmissionGameUserSettings::SetSecondaryKey(const FKey& NewKey)
{
    SecondaryKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetSecondaryKey() const
{
    return SecondaryKey;
}

void UWildOmissionGameUserSettings::SetInteractKey(const FKey& NewKey)
{
    InteractKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetInteractKey() const
{
    return InteractKey;
}

void UWildOmissionGameUserSettings::SetReloadKey(const FKey& NewKey)
{
    ReloadKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetReloadKey() const
{
    return ReloadKey;
}

void UWildOmissionGameUserSettings::SetInventoryKey(const FKey& NewKey)
{
    InventoryKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetInventoryKey() const
{
    return InventoryKey;
}

void UWildOmissionGameUserSettings::SetCraftingKey(const FKey& NewKey)
{
    CraftingKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetCraftingKey() const
{
    return CraftingKey;
}

void UWildOmissionGameUserSettings::SetChatKey(const FKey& NewKey)
{
    ChatKey = NewKey;
}

FKey UWildOmissionGameUserSettings::GetChatKey() const
{
    return ChatKey;
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
