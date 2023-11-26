// Copyright Telephone Studios. All Rights Reserved.


#include "Items/LockApplicationItem.h"
#include "Components/LockComponent.h"
#include "Locks/Lock.h"
#include "UObject/ConstructorHelpers.h"

ALockApplicationItem::ALockApplicationItem()
{
	LockMesh = nullptr;

	ConstructorHelpers::FObjectFinder<UStaticMesh> LockMeshObject(TEXT("/Game/Locks/Art/SM_CodeLock"));
	if (LockMeshObject.Succeeded())
	{
		LockMesh = LockMeshObject.Object;
	}
}

void ALockApplicationItem::OnPlace()
{
	FHitResult HitResult;
	if (!LineTraceOnChannel(ECollisionChannel::ECC_Visibility, HitResult))
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
	{
		return;
	}

	ULockComponent* HitLockComponent = HitActor->FindComponentByClass<ULockComponent>();
	if (HitLockComponent == nullptr || HitLockComponent->IsLockPlaced())
	{
		return;
	}

	HitLockComponent->ApplyLock();

	Super::OnPlace();
}

bool ALockApplicationItem::GetPlacementTransform(FTransform& OutPlacementTransform)
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
	if (HitLockComponent == nullptr || HitLockComponent->IsLockPlaced())
	{
		return false;
	}
	
	OutPlacementTransform = HitLockComponent->GetComponentTransform();
	if (!HasBuildingPrivilege(HitLockComponent->GetComponentLocation()))
	{
		return false;
	}

	return true;
}

UStaticMesh* ALockApplicationItem::GetPreviewMesh()
{
	return LockMesh;
}
