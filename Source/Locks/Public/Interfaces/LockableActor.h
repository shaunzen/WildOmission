// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockableActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULockableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOCKS_API ILockableActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void ApplyLock();
	void RemoveLock();

	void SetCode(const FString& NewCode);
	void ClearCode();

	void AddAuthorizedPlayer(const FString& PlayerToAuthorize);

	bool IsPlayerAuthorized(const FString& Player) const;

private:
	UPROPERTY(EditDefaultsOnly)
	FVector LockMountLocation = FVector::ZeroVector;

	UPROPERTY(SaveGame)
	bool HasLock = false;

	UPROPERTY(SaveGame)
	TArray<FString> AuthorizedPlayers;

	UPROPERTY(SaveGame)
	TOptional<FString> Code;

};
