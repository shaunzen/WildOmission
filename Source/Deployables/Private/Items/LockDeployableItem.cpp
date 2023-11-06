// Copyright Telephone Studios. All Rights Reserved.


#include "Items/LockDeployableItem.h"
#include "Components/LockComponent.h"

void ALockDeployableItem::OnPlace()
{

	// TODO get the thing we are looking at
	// TODO set has lock to true

	Super::OnPlace();
}

bool ALockDeployableItem::GetPlacementTransform(FTransform& OutPlacementTransform)
{
	Super::GetPlacementTransform(OutPlacementTransform);

	FHitResult HitResult;
	if (!LineTraceOnChannel(ECollisionChannel::ECC_Visibility, HitResult))
	{
		return false;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
	{
		return false;
	}

	ULockComponent* HitLockComponent = HitActor->FindComponentByClass<ULockComponent>();
	if (HitLockComponent == nullptr)
	{
		return false;
	}

	if (HitLockComponent->IsLockPlaced())
	{
		return false;
	}

	OutPlacementTransform = HitLockComponent->GetComponentTransform();
	return true;
}
