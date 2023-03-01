// Copyright Telephone Studios. All Rights Reserved.


#include "HumanAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Components/EquipComponent.h"

UHumanAnimInstance::UHumanAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	Speed = 0;
	Angle = 0;
	Falling = false;
	HoldingItem = false;

	ConstructorHelpers::FObjectFinder<USoundBase> GrassFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Grass/HumanFootstep_Grass_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> GravelFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Gravel/HumanFootstep_Gravel_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> RockFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Rock/HumanFootstep_Rock_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> WoodFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Wood/HumanFootstep_Wood_Cue"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> SwingAnimMontageObject(TEXT("/Game/WildOmission/Characters/Human/Animation/A_Human_SwingTool_02_Montage"));

	if (GrassFootstepSoundObject.Object == nullptr
		|| GravelFootstepSoundObject.Object == nullptr
		|| RockFootstepSoundObject.Object == nullptr
		|| WoodFootstepSoundObject.Object == nullptr
		|| SwingAnimMontageObject.Object == nullptr)
	{
		return;
	}

	GrassFootstepSound = GrassFootstepSoundObject.Object;
	GravelFootstepSound = GravelFootstepSoundObject.Object;
	RockFootstepSound = RockFootstepSoundObject.Object;
	WoodFootstepSound = WoodFootstepSoundObject.Object;
	SwingMontage = SwingAnimMontageObject.Object;
}

void UHumanAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	CalculateSpeedAndAngle();
	HandleFalling();
	HandleItemHolding();
}

void UHumanAnimInstance::PlaySwingAnimation()
{
	if (Montage_IsPlaying(SwingMontage))
	{
		return;
	}
	Montage_Play(SwingMontage);
}

void UHumanAnimInstance::PlayFootstepSound()
{
	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	FVector StartLocation = PawnOwner->GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 100.0f);
	FCollisionQueryParams Params;

	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params))
	{
		if (HitResult.PhysMaterial == nullptr)
		{
			return;
		}

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
		case SurfaceType_Default:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RockFootstepSound, HitResult.Location);
			break;
		}
	}
}

void UHumanAnimInstance::CalculateSpeedAndAngle()
{
	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner == nullptr)
	{
		return;
	}

	Speed = PawnOwner->GetVelocity().Length();
	Angle = PawnOwner->GetTransform().InverseTransformVector(PawnOwner->GetVelocity()).Rotation().Yaw;
}

void UHumanAnimInstance::HandleFalling()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(TryGetPawnOwner());
	if (CharacterOwner == nullptr)
	{
		return;
	}

	Falling = CharacterOwner->GetCharacterMovement()->IsFalling();
}

void UHumanAnimInstance::HandleItemHolding()
{
	if (TryGetPawnOwner() == nullptr)
	{
		return;
	}

	UEquipComponent* PlayerEquipComponent = TryGetPawnOwner()->FindComponentByClass<UEquipComponent>();
	if (PlayerEquipComponent == nullptr)
	{
		return;
	}

	HoldingItem = PlayerEquipComponent->IsItemEquiped();
}