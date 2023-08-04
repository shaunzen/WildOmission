// Copyright Telephone Studios. All Rights Reserved.


#include "HumanAnimInstance.h"
#include "Components/EquipComponent.h"
#include "Items/ToolItem.h"
#include "Weapons/Firearm.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"

UHumanAnimInstance::UHumanAnimInstance(const FObjectInitializer& ObjectInitializer) : UWildOmissionAnimInstance(ObjectInitializer)
{
	HeadAngle = 0.0f;
	EquipedItemPose = nullptr;
	FirstPersonInstance = false;
}

void UHumanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CalculateHeadAngle();
	HandleEquipedItemPose();
}

void UHumanAnimInstance::OnPrimaryMontageClimax()
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

	EquipedTool->OnPrimaryAnimationClimax(FirstPersonInstance);
}

void UHumanAnimInstance::OnReloadMontageClimax()
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

	AFirearm* EquipedFirearm = Cast<AFirearm>(OwnerEquipComponent->GetEquipedItem());
	if (EquipedFirearm == nullptr)
	{
		return;
	}

	EquipedFirearm->OnReloadAnimationClimax(FirstPersonInstance);
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
	if (TryGetPawnOwner() == nullptr)
	{
		return;
	}

	UEquipComponent* PlayerEquipComponent = TryGetPawnOwner()->FindComponentByClass<UEquipComponent>();
	if (PlayerEquipComponent == nullptr)
	{
		return;
	}

	if (TryGetPawnOwner()->IsLocallyControlled())
	{
		if (PlayerEquipComponent->GetLocalEquipedItemDefaultClass() == nullptr)
		{
			EquipedItemPose = nullptr;
			return;
		}

		EquipedItemPose = PlayerEquipComponent->GetLocalEquipedItemDefaultClass()->GetEquipPose();
	}
	else
	{
		if (PlayerEquipComponent->GetEquipedItem() == nullptr)
		{
			EquipedItemPose = nullptr;
			return;
		}

		EquipedItemPose = PlayerEquipComponent->GetEquipedItem()->GetEquipPose();
	}
}