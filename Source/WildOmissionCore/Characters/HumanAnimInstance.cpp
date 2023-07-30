// Copyright Telephone Studios. All Rights Reserved.


#include "HumanAnimInstance.h"
#include "Components/EquipComponent.h"
#include "Items/ToolItem.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"

UHumanAnimInstance::UHumanAnimInstance(const FObjectInitializer& ObjectInitializer) : UWildOmissionAnimInstance(ObjectInitializer)
{
	FirstPersonInstance = false;
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

float UHumanAnimInstance::GetHeadAngle() const
{
	AWildOmissionCharacter* CharacterOwner = Cast<AWildOmissionCharacter>(TryGetPawnOwner());
	if (CharacterOwner == nullptr)
	{
		return 0.0f;
	}

	return -CharacterOwner->GetHeadPitch() * 0.5f;
}

UAnimSequence* UHumanAnimInstance::GetEquipedItemPose() const
{
	if (TryGetPawnOwner() == nullptr)
	{
		return nullptr;
	}

	UEquipComponent* PlayerEquipComponent = TryGetPawnOwner()->FindComponentByClass<UEquipComponent>();
	if (PlayerEquipComponent == nullptr)
	{
		return nullptr;
	}

	if (TryGetPawnOwner()->IsLocallyControlled())
	{
		if (PlayerEquipComponent->GetLocalEquipedItemDefaultClass() == nullptr)
		{
			return nullptr;
		}

		return PlayerEquipComponent->GetLocalEquipedItemDefaultClass()->GetEquipPose();
	}
	else
	{
		if (PlayerEquipComponent->GetEquipedItem() == nullptr)
		{
			return nullptr;
		}

		return PlayerEquipComponent->GetEquipedItem()->GetEquipPose();
	}
}