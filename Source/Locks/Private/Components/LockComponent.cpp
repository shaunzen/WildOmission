// Copyright Telephone Studios. All Rights Reserved.


#include "Components/LockComponent.h"

// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULockComponent::ApplyLock()
{
	// Set Visible
}

void ULockComponent::RemoveLock()
{
	// Set Hidden
}

void ULockComponent::SetCode(const FString& NewCode)
{
	Code = NewCode;
	AuthorizedPlayers.Empty();
}

void ULockComponent::ClearCode()
{
	Code.Reset();
}

FString ULockComponent::GetCode() const
{
	return Code.GetValue();
}

void ULockComponent::AddAuthorizedPlayer(const FString& PlayerToAuthorize)
{
	AuthorizedPlayers.Add(PlayerToAuthorize);
}

bool ULockComponent::IsPlayerAuthorized(const FString& PlayerUniqueID) const
{
	for (const FString& AuthorizedPlayer : AuthorizedPlayers)
	{
		if (AuthorizedPlayer != PlayerUniqueID)
		{
			continue;
		}
	
		return true;
	}

	return false;
}

