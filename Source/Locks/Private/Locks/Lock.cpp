// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/LockDeployable.h"
#include "Interfaces/LockModifier.h"

void ALockDeployable::Interact(AActor* Interactor)
{
	ILockModifier* LockModifierInteractor = Cast<ILockModifier>(Interactor);
	if (LockModifierInteractor == nullptr)
	{
		return;
	}

	LockModifierInteractor->OpenLockMenu(this);
}

FString ALockDeployable::PromptText()
{
	return TEXT("open keypad");
}
