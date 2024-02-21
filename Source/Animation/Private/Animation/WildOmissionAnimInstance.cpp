// Copyright Telephone Studios. All Rights Reserved.


#include "Animation/WildOmissionAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Chunk.h"
#include "ChunkManager.h"
#include "SurfaceHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

static USoundBase* WaterSplashSound = nullptr;

UWildOmissionAnimInstance::UWildOmissionAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	Speed = 0.0f;
	Angle = 0.0f;
	Falling = false;
	Swimming = false;

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
	UWorld* World = GetWorld();
	APawn* PawnOwner = TryGetPawnOwner();
	if (World == nullptr || PawnOwner == nullptr)
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

	if (!World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params))
	{
		return;
	}

	USoundBase* FootstepSound = nullptr;
	if (AChunk* HitChunk = Cast<AChunk>(HitResult.GetActor()))
	{
		// Get Chunk Manager
		AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
		if (ChunkManager == nullptr)
		{
			return;
		}

		// Set footstep sound
		const uint8 SurfaceType = ChunkManager->GetSurfaceTypeAtLocation(HitResult.ImpactPoint);
		FootstepSound = USurfaceHelpers::GetFootstepSound(SurfaceType);
		
	}
	else
	{
		// Get hit physical material
		if (HitResult.PhysMaterial == nullptr)
		{
			return;
		}

		// Set footstep sound
		FootstepSound = USurfaceHelpers::GetFootstepSound(HitResult.PhysMaterial->SurfaceType);
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSound, HitResult.ImpactPoint);
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