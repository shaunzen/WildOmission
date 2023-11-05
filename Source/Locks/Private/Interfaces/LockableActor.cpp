// Copyright Telephone Studios. All Rights Reserved.


#include "Interfaces/LockableActor.h"
#include "Components/CodeLockComponent.h"

// Add default functionality here for any ILockableActor functions that are not pure virtual.

void ILockableActor::ApplyLock()
{
	// Get This Actor
	AActor* ThisActor = Cast<AActor>(this);
	if (ThisActor)
	{
		return;
	}

	// Create New Component
	UCodeLockComponent* NewCodeLock = NewObject<UCodeLockComponent>();
	if (NewCodeLock == nullptr)
	{
		return;
	}
	
	NewCodeLock->RegisterComponent();

	
	// Attach To Root
	NewCodeLock->AttachToComponent(ThisActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Set Relative Location
	NewCodeLock->SetRelativeLocation(LockMountLocation);

	// Set HasLock True
	HasLock = true;
}

void ILockableActor::RemoveLock()
{
	// Get This Actor
	AActor* ThisActor = Cast<AActor>(this);
	if (ThisActor == nullptr)
	{
		return;
	}

	// Get Existing Lock Component
	UCodeLockComponent* ExistingCodeLock = ThisActor->FindComponentByClass<UCodeLockComponent>();
	if (ExistingCodeLock == nullptr)
	{
		return;
	}

	// Destroy It
	ExistingCodeLock->DestroyComponent();

	// Set HasLock False
	HasLock = false;
}

void ILockableActor::SetCode(const FString& NewCode)
{
	Code = NewCode;
	AuthorizedPlayers.Empty();
}

void ILockableActor::ClearCode()
{
	Code.Reset();
}

void ILockableActor::AddAuthorizedPlayer(const FString& PlayerToAuthorize)
{
	AuthorizedPlayers.Add(PlayerToAuthorize);
}

bool ILockableActor::IsPlayerAuthorized(const FString& Player) const
{
	for (const FString& AuthorizedPlayer : AuthorizedPlayers)
	{
		if (AuthorizedPlayer != Player)
		{
			continue;
		}

		return true;
	}

	return false;
}
