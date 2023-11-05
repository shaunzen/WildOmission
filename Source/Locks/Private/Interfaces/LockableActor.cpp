// Copyright Telephone Studios. All Rights Reserved.


#include "Interfaces/LockableActor.h"

// Add default functionality here for any ILockableActor functions that are not pure virtual.

void ILockableActor::ApplyLock()
{
	// Create New Component

	// Attach to us

	// Set Relative Location

	HasLock = true;
}

void ILockableActor::RemoveLock()
{
	// Get existing component

	// destroy it

	// Set has lock false

	HasLock = false;
}

void ILockableActor::SetCode(const FString& NewCode)
{
	Code = NewCode;
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
