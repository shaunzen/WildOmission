// Copyright Telephone Studios. All Rights Reserved.


#include "HumanAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Items/ToolItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

UHumanAnimInstance::UHumanAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	Speed = 0;
	Angle = 0;
	Falling = false;
	EquipedItemPose = nullptr;

	ConstructorHelpers::FObjectFinder<USoundBase> GrassFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Grass/HumanFootstep_Grass_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> GravelFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Gravel/HumanFootstep_Gravel_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> RockFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Rock/HumanFootstep_Rock_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> WoodFootstepSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Footsteps/Wood/HumanFootstep_Wood_Cue"));
	
	if (GrassFootstepSoundObject.Object == nullptr
		|| GravelFootstepSoundObject.Object == nullptr
		|| RockFootstepSoundObject.Object == nullptr
		|| WoodFootstepSoundObject.Object == nullptr)
	{
		return;
	}

	GrassFootstepSound = GrassFootstepSoundObject.Object;
	GravelFootstepSound = GravelFootstepSoundObject.Object;
	RockFootstepSound = RockFootstepSoundObject.Object;
	WoodFootstepSound = WoodFootstepSoundObject.Object;
	StopWalkAnimationWhenFalling = false;
}

void UHumanAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	CalculateSpeedAndAngle();
	CalculateHeadAngle();
	HandleFalling();
	HandleEquipPose();
}

void UHumanAnimInstance::PlayMontage(UAnimMontage* Montage)
{
	if (Montage_IsPlaying(Montage))
	{
		return;
	}
	Montage_Play(Montage);
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

void UHumanAnimInstance::HarvestResource()
{
	// get owner equip comp and tell it to harvest
	APawn* PawnOwner = TryGetPawnOwner();
	if (PawnOwner == nullptr)
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = PawnOwner->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr) 
	{
		return;
	}

	AToolItem* EquipedTool = Cast<AToolItem>(OwnerEquipComponent->GetEquipedItem());
	if (EquipedTool == nullptr)
	{
		return;
	}

	EquipedTool->Harvest();
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
	
	if (StopWalkAnimationWhenFalling == true && Falling == true)
	{
		Speed = 0.0f;
		Angle = 0.0f;
	}
}

void UHumanAnimInstance::CalculateHeadAngle()
{
	AWildOmissionCharacter* CharacterOwner = Cast<AWildOmissionCharacter>(TryGetPawnOwner());
	if (CharacterOwner == nullptr)
	{
		return;
	}
	
	HeadAngle = -CharacterOwner->GetHeadPitch() * 0.5f;	
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

void UHumanAnimInstance::HandleEquipPose()
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

	if (PlayerEquipComponent->GetEquipedItem() == nullptr)
	{
		EquipedItemPose = nullptr;
		return;
	}

	EquipedItemPose = PlayerEquipComponent->GetEquipedItem()->GetEquipPose();	
}