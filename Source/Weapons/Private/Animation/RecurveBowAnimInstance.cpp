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
