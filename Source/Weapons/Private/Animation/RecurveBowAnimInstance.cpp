// Copyright Telephone Studios. All Rights Reserved.


#include "Animation/RecurveBowAnimInstance.h"
#include "Components/EquipComponent.h"
#include "Items/BowItem.h"

URecurveBowAnimInstance::URecurveBowAnimInstance()
{
	Drawn = false;
}

void URecurveBowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	HandleDrawn();
	HandleArrowVisibility();
}

void URecurveBowAnimInstance::HandleDrawn()
{
	ABowItem* EquipedBow = GetEquipedBow();
	if (EquipedBow == nullptr)
	{
		return;
	}
	Drawn = EquipedBow->IsDrawn();
}

void URecurveBowAnimInstance::HandleArrowVisibility()
{
	ABowItem* EquipedBow = GetEquipedBow();
	if (EquipedBow == nullptr || GetSkelMeshComponent() == nullptr)
	{
		return;
	}

	if (!EquipedBow->HasAmmo())
	{
		GetSkelMeshComponent()->HideBoneByName(TEXT("Arrow"), EPhysBodyOp::PBO_None);
		return;
	}

	GetSkelMeshComponent()->UnHideBoneByName(TEXT("Arrow"));
}

ABowItem* URecurveBowAnimInstance::GetEquipedBow() const
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn == nullptr)
	{
		return Cast<ABowItem>(GetOwningActor());
	}

	UEquipComponent* OwnerEquipComponent = OwnerPawn->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return nullptr;
	}

	ABowItem* EquipedBow = Cast<ABowItem>(OwnerEquipComponent->GetEquipedItem());
	
	return EquipedBow;
}
