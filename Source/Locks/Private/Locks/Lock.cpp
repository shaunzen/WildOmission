// Copyright Telephone Studios. All Rights Reserved.


#include "Locks/Lock.h"
#include "Components/LockModifierComponent.h"

void ALock::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		return;
	}

	ULockModifierComponent* InteractorLockModifierComponent = Interactor->FindComponentByClass<ULockModifierComponent>();
	if (InteractorLockModifierComponent == nullptr)
	{
		return;
	}

	InteractorLockModifierComponent->OpenKeypadMenu(this);
}

FString ALock::PromptText()
{
	return TEXT("open keypad");
}
