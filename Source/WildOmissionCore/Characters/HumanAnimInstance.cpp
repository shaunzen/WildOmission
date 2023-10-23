// Copyright Telephone Studios. All Rights Reserved.


#include "HumanAnimInstance.h"
#include "Components/EquipComponent.h"
#include "Items/ToolItem.h"
#include "Items/FirearmItem.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"

UHumanAnimInstance::UHumanAnimInstance(const FObjectInitializer& ObjectInitializer) : UWildOmissionAnimInstance(ObjectInitializer)
{
	HeadAngle = 0.0f;
	EquipedItemPose = nullptr;
	
	RightArmOffset = FRotator::ZeroRotator;
	LeftArmOffset = FRotator::ZeroRotator;

	FirstPersonInstance = false;
}

void UHumanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CalculateHeadAngle();
	HandleEquipedItemPose();
	HandleArmOffset();
}

void UHumanAnimInstance::OnPrimaryAnimationClimax()
{
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

	OwnerEquipComponent->OnPrimaryAnimationClimax(FirstPersonInstance);
}

void UHumanAnimInstance::OnSecondaryAnimationClimax()
{
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

	OwnerEquipComponent->OnSecondaryAnimationClimax(FirstPersonInstance);
}

void UHumanAnimInstance::OnReloadAnimationClimax()
{
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

	OwnerEquipComponent->OnReloadAnimationClimax(FirstPersonInstance);
}

void UHumanAnimInstance::CalculateSpeedAndAngle()
{
	Super::CalculateSpeedAndAngle();

	if (FirstPersonInstance && (Falling || Swimming))
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

	HeadAngle = CharacterOwner->GetHeadPitch() * 0.5f;
}

void UHumanAnimInstance::HandleEquipedItemPose()
{
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

	EquipedItemPose = OwnerEquipComponent->GetEquipedItemPose();
}

void UHumanAnimInstance::HandleArmOffset()
{
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

	const float PawnVelocity = FVector(PawnOwner->GetVelocity().X, PawnOwner->GetVelocity().Y, 0.0f).Length();
	const float MaxPawnVelocity = 600.0f;
	const float NormalizedPawnVelocity = PawnVelocity / MaxPawnVelocity;

	RightArmOffset = OwnerEquipComponent->GetEquipedItemRightArmOffset() * NormalizedPawnVelocity;
	LeftArmOffset = OwnerEquipComponent->GetEquipedItemLeftArmOffset() * NormalizedPawnVelocity;
}
