// Copyright Telephone Studios. All Rights Reserved.


#include "Components/LockComponent.h"
#include "Deployables/LockDeployable.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	HasLock = false;
	SpawnedLock = nullptr;

	static ConstructorHelpers::FClassFinder<ALockDeployable> LockBlueprint(TEXT("/Game/Deployables/BP_CodeLock"));
	if (LockBlueprint.Succeeded())
	{
		LockClass = LockBlueprint.Class;
	}
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
	// Spawn Lock
	if (HasLock && SpawnedLock == nullptr)
	{
		SpawnedLock = GetWorld()->SpawnActor<ALockDeployable>(LockClass);
		if (SpawnedLock == nullptr)
		{
			return;
		}
		SpawnedLock->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	// Remove Lock
	else if (!HasLock && SpawnedLock)
	{
		SpawnedLock->Destroy();
		SpawnedLock = nullptr;
	}
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULockComponent::ApplyLock()
{
	HasLock = true;
	OnRep_HasLock();
}

void ULockComponent::RemoveLock()
{
	HasLock = false;
	OnRep_HasLock();
}

bool ULockComponent::IsLockPlaced() const
{
	return HasLock;
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

