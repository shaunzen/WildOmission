// Copyright Telephone Studios. All Rights Reserved.


#include "Items/LockDeployableItem.h"
#include "Components/LockComponent.h"

// TODO I feel like it should be the other way around, returns bool, and out parameter is transform
FTransform ALockDeployableItem::GetPlacementTransform(bool& OutValidSpawn)
{
	FHitResult HitResult;
	if (!LineTraceOnChannel(ECollisionChannel::ECC_Visibility, HitResult))
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}

	ULockComponent* HitLockComponent = HitActor->FindComponentByClass<ULockComponent>();
	if (HitLockComponent == nullptr)
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}

	if (HitLockComponent->IsLockPlaced())
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}

	OutValidSpawn = true;
	return HitLockComponent->GetComponentTransform();
}
