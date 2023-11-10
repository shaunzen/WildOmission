// Copyright Telephone Studios. All Rights Reserved.


#include "Components/LockComponent.h"
#include "Locks/Lock.h"
#include "GameFramework/PlayerState.h"
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

	static ConstructorHelpers::FClassFinder<ALock> CodeLockBlueprint(TEXT("/Game/Locks/Locks/BP_CodeLock"));
	if (CodeLockBlueprint.Succeeded())
	{
		CodeLockClass = CodeLockBlueprint.Class;
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

	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	OwnerActor->OnDestroyed.AddDynamic(this, &ULockComponent::OnOwnerDestroyed);
}

void ULockComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
	RemoveLock();
}

void ULockComponent::OnRep_HasLock()
{
	// Spawn Lock
	if (HasLock && SpawnedLock == nullptr)
	{
		SpawnedLock = GetWorld()->SpawnActor<ALock>(CodeLockClass);
		if (SpawnedLock == nullptr)
		{
			return;
		}
		SpawnedLock->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SpawnedLock->OnPlacement(this);
		SpawnedLock->SetAuthorizedPlayers(CodeLockSave.AuthorizedPlayers);
		SpawnedLock->SetCode(CodeLockSave.Code);
	}
	// Remove Lock
	else if (!HasLock && SpawnedLock)
	{
		SpawnedLock->Destroy();
		SpawnedLock = nullptr;
	}
}

void ULockComponent::OnLoadComplete_Implementation()
{
	OnRep_HasLock();
}

// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULockComponent::Serialize(FArchive& Ar)
{
	if (Ar.IsSaving())
	{
		CodeLockSave.HasLock = HasLock;
		if (IsValid(SpawnedLock))
		{
			CodeLockSave.Code = SpawnedLock->GetCode();
			CodeLockSave.AuthorizedPlayers = SpawnedLock->GetAuthorizedPlayers();
		}
		else
		{
			CodeLockSave.Code.Empty();
			CodeLockSave.AuthorizedPlayers.Empty();
		}
	}
	Super::Serialize(Ar);
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
	return HasLock && IsValid(GetLock());
}

ALock* ULockComponent::GetLock() const
{
	return SpawnedLock;
}
