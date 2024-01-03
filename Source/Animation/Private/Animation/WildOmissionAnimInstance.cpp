// Copyright Telephone Studios. All Rights Reserved.


#include "Animation/WildOmissionAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

static USoundBase* GrassFootstepSound = nullptr;
static USoundBase* GravelFootstepSound = nullptr;
static USoundBase* RockFootstepSound = nullptr;
static USoundBase* WoodFootstepSound = nullptr;
static USoundBase* WaterSplashSound = nullptr;

UWildOmissionAnimInstance::UWildOmissionAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	Speed = 0.0f;
	Angle = 0.0f;
	Falling = false;
	Swimming = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> GrassFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Grass/Footstep_Grass_Cue"));
	if (GrassFootstepSoundObject.Succeeded())
	{
		GrassFootstepSound = GrassFootstepSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> GravelFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Gravel/Footstep_Gravel_Cue"));
	if (GravelFootstepSoundObject.Succeeded())
	{
		GravelFootstepSound = GravelFootstepSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> RockFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Rock/Footstep_Rock_Cue"));
	if (RockFootstepSoundObject.Succeeded())
	{
		RockFootstepSound = RockFootstepSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> WoodFootstepSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Wood/Footstep_Wood_Cue"));
	if (WoodFootstepSoundObject.Succeeded())
	{
		WoodFootstepSound = WoodFootstepSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> WaterSplashSoundObject(TEXT("/Game/WildOmissionCore/Audio/Footsteps/Water/WaterSplash_Cue"));
	if (WaterSplashSoundObject.Succeeded())
	{
		WaterSplashSound = WaterSplashSoundObject.Object;
	}
}

void UWildOmissionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CalculateSpeedAndAngle();
	HandleFalling();
	HandleSwimming();
}

void UWildOmissionAnimInstance::PlayMontage(UAnimMontage* Montage, float MontagePlayRate)
{
	if (Montage_IsPlaying(Montage))
	{
		return;
	}

	Montage_Play(Montage, MontagePlayRate);
}

void UWildOmissionAnimInstance::PlayFootstepSound()
{
	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner == nullptr)
	{
		return;
	}

	if (Swimming == true)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WaterSplashSound, PawnOwner->GetActorLocation());
		return;
	}

	FHitResult HitResult;
	FVector StartLocation = PawnOwner->GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 100.0f);
	FCollisionQueryParams Params;

	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(PawnOwner);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params))
	{
		if (HitResult.PhysMaterial == nullptr)
		{
			return;
		}

		// TODO if ground use surface on ground

		switch (HitResult.PhysMaterial->SurfaceType)
		{
		case SurfaceType1: // Grass
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrassFootstepSound, HitResult.Location);
			break;
		case SurfaceType2: // Gravel
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GravelFootstepSound, HitResult.Location);
			break;
		case SurfaceType3: // Rock
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RockFootstepSound, HitResult.Location);
			break;
		case SurfaceType4: // Wood
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WoodFootstepSound, HitResult.Location);
			break;
		case SurfaceType5: // Leaves
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrassFootstepSound, HitResult.Location);
			break;
		case SurfaceType6: // Cloth
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WoodFootstepSound, HitResult.Location);
			break;
		case SurfaceType7: // Metal
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RockFootstepSound, HitResult.Location);
			break;
		case SurfaceType_Default:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RockFootstepSound, HitResult.Location);
			break;
		}
	}
}

void UWildOmissionAnimInstance::CalculateSpeedAndAngle()
{
	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner == nullptr)
	{
		return;
	}

	Speed = PawnOwner->GetVelocity().Length();
	Angle = PawnOwner->GetTransform().InverseTransformVector(PawnOwner->GetVelocity()).Rotation().Yaw;
}

void UWildOmissionAnimInstance::HandleFalling()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(TryGetPawnOwner());
	if (CharacterOwner == nullptr)
	{
		return;
	}

	Falling = CharacterOwner->GetCharacterMovement()->IsFalling();
}

void UWildOmissionAnimInstance::HandleSwimming()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(TryGetPawnOwner());
	if (CharacterOwner == nullptr)
	{
		return;
	}

	Swimming = CharacterOwner->GetCharacterMovement()->IsSwimming();
}