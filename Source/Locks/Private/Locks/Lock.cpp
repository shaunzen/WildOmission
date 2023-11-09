// Copyright Telephone Studios. All Rights Reserved.


#include "Locks/Lock.h"
#include "Components/LockComponent.h"
#include "GameFramework/PlayerState.h"
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

void ALock::SetCode(const FString& NewCode, const FString& CodeSetterUniqueID)
{
	if (Code.Len() < 4)
	{
		return;
	}

	AuthorizedPlayers.Empty();
	Code = NewCode;
	if (!CodeSetterUniqueID.IsEmpty())
	{
		AuthorizedPlayers.Add(CodeSetterUniqueID);
	}
}

void ALock::ClearCode()
{
	Code.Empty();
}

FString ALock::GetCode() const
{
	return Code;
}

TArray<FString> ALock::GetAuthorizedPlayers() const
{
	return AuthorizedPlayers;
}

void ALock::SetAuthorizedPlayers(const TArray<FString>& InAuthorizedPlayers)
{
	AuthorizedPlayers = InAuthorizedPlayers;
}

bool ALock::IsLocked() const
{
	return Code.Len() > 0;
}

void ALock::AuthorizePlayer(const FString& PlayerUniqueID)
{
	AuthorizedPlayers.Add(PlayerUniqueID);
}

bool ALock::IsAuthorized(const FString& PlayerUniqueID) const
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

bool ALock::IsAuthorized(ULockModifierComponent* LockModifier) const
{
	if (OwnerLockComponent == nullptr || LockModifier == nullptr)
	{
		return false;
	}

	APawn* PlayerPawn = Cast<APawn>(LockModifier->GetOwner());
	if (PlayerPawn == nullptr)
	{
		return false;
	}

	return IsAuthorized(PlayerPawn);
}

bool ALock::IsAuthorized(APawn* PlayerPawn) const
{
	if (PlayerPawn == nullptr)
	{
		return false;
	}

	APlayerState* OwnerPlayerState = PlayerPawn->GetPlayerState();
	if (OwnerPlayerState == nullptr)
	{
		return false;
	}

	return IsAuthorized(OwnerPlayerState->GetUniqueId().ToString());
}

ULockComponent* ALock::GetOwnerLockComponent() const
{
	return OwnerLockComponent;
}

UStaticMesh* ALock::GetStaticMesh() const
{
	return MeshComponent->GetStaticMesh();
}
