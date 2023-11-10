// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/StorageBox.h"
#include "Locks/Lock.h"
#include "Components/LockComponent.h"

AStorageBox::AStorageBox()
{
	LockComponent = nullptr;

	LockComponent = CreateDefaultSubobject<ULockComponent>(TEXT("LockComponent"));
	LockComponent->SetupAttachment(MeshComponent);
}

bool AStorageBox::CanOpen(AActor* Interactor) const
{
	Super::CanOpen(Interactor);

	if (Interactor == nullptr || LockComponent == nullptr)
	{
		return false;
	}

	if (IsValid(LockComponent->GetLock()) && !LockComponent->GetLock()->IsAuthorized(Cast<APawn>(Interactor)) && LockComponent->GetLock()->IsLocked())
	{
		return false;
	}

	return true;
}
