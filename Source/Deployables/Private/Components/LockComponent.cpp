// Copyright Telephone Studios. All Rights Reserved.


#include "Components/LockComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	LockMesh = nullptr;
	HasLock = false;

}

void ULockComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULockComponent, HasLock)
}

// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();

}

void ULockComponent::OnRep_HasLock()
{
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULockComponent::ApplyLock()
{
	// TODO Apply Lock
}

void ULockComponent::RemoveLock()
{
	// TODO Remove Lock
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
	return Code;
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

