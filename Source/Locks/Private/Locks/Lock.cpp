// Copyright Telephone Studios. All Rights Reserved.


#include "Locks/Lock.h"
#include "Components/LockComponent.h"
#include "Components/LockModifierComponent.h"

ALock::ALock()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ALock::Setup(ULockComponent* InOwnerLockComponent)
{
	OwnerLockComponent = InOwnerLockComponent;
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

FString ALock::GetCode() const
{
	if (OwnerLockComponent == nullptr)
	{
		return TEXT("");
	}

	return OwnerLockComponent->GetCode();
}

bool ALock::IsLocked() const
{
	if (OwnerLockComponent == nullptr)
	{
		return true;
	}

	return OwnerLockComponent->GetCode() == TEXT("");
}

bool ALock::IsPlayerAuthorized(const FString& PlayerUniqueID) const
{
	if (OwnerLockComponent == nullptr)
	{
		return false;
	}

	return OwnerLockComponent->IsPlayerAuthorized(PlayerUniqueID);
}

UStaticMesh* ALock::GetStaticMesh() const
{
	return MeshComponent->GetStaticMesh();
}
