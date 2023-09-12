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
	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn == nullptr)
	{
		Drawn = false;
		return;
	}

	UEquipComponent* OwnerEquipComponent = OwnerPawn->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		Drawn = false;
		return;
	}

	ABowItem* EquipedBow = Cast<ABowItem>(OwnerEquipComponent->GetEquipedItem());
	if (EquipedBow == nullptr)
	{
		Drawn = false;
		return;
	}

	Drawn = EquipedBow->IsDrawn();
}
