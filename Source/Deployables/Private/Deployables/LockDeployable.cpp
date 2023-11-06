// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/LockDeployable.h"

void ALockDeployable::Interact(AActor* Interactor)
{
	// TODO bring up menu
	// Cast to LockInterface
	// LockInterface->BringUpLockMenu(this);
	// some method for getting the client to open the widget
}

FString ALockDeployable::PromptText()
{
	return TEXT("open keypad");
}
