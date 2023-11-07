// Copyright Telephone Studios. All Rights Reserved.


#include "Locks/Lock.h"
#include "Components/LockModifierComponent.h"

ALock::ALock()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

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

UStaticMesh* ALock::GetStaticMesh() const
{
	return MeshComponent->GetStaticMesh();
}
