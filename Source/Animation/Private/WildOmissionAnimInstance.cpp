// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionAnimInstance.h"
#include "UObject/ConstructorHelpers.h"

UWildOmissionAnimInstance::UWildOmissionAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	Speed = 0.0f;
	Angle = 0.0f;
	Falling = false;
	Swimming = false;

	ConstructorHelpers::FObjectFinder<USoundBase> GrassFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Grass/HumanFootstep_Grass_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> GravelFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Gravel/HumanFootstep_Gravel_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> RockFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Rock/HumanFootstep_Rock_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> WoodFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Wood/HumanFootstep_Wood_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> WaterSplashSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Water/WaterSplash_Cue"));

}

void UWildOmissionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
}

void UWildOmissionAnimInstance::PlayFootstepSound()
{
}

float UWildOmissionAnimInstance::GetSpeed() const
{
	return 0.0f;
}

float UWildOmissionAnimInstance::GetAngle() const
{
	return 0.0f;
}

bool UWildOmissionAnimInstance::IsFalling() const
{
	return false;
}

bool UWildOmissionAnimInstance::IsSwimming() const
{
	return false;
}

void UWildOmissionAnimInstance::CalculateSpeedAndAngle()
{
}

void UWildOmissionAnimInstance::HandleFalling()
{
}

void UWildOmissionAnimInstance::HandleSwimming()
{
}
