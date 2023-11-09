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
	if (OwnerLockComponent == nullptr)
	{
		return;
	}
	OwnerLockComponent->SetCode(NewCode);
	OwnerLockComponent->AddAuthorizedPlayer(CodeSetterUniqueID);
}

void ALock::ClearCode()
{
	if (OwnerLockComponent == nullptr)
	{
		return;
	}

	OwnerLockComponent->ClearCode();
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

	return OwnerLockComponent->GetCode().Len() > 0;
}

void ALock::AuthorizePlayer(const FString& PlayerUniqueID)
{
	if (OwnerLockComponent == nullptr)
	{
		return;
	}

	OwnerLockComponent->AddAuthorizedPlayer(PlayerUniqueID);
}

bool ALock::IsAuthorized(const FString& PlayerUniqueID) const
{
	if (OwnerLockComponent == nullptr)
	{
		return false;
	}

	return OwnerLockComponent->IsPlayerAuthorized(PlayerUniqueID);
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

	APlayerState* PlayerState = PlayerPawn->GetPlayerState();
	if (PlayerState == nullptr)
	{
		return false;
	}

	return OwnerLockComponent->IsPlayerAuthorized(PlayerState->GetUniqueId().ToString());
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

	return OwnerLockComponent->IsPlayerAuthorized(OwnerPlayerState->GetUniqueId().ToString());
}

UStaticMesh* ALock::GetStaticMesh() const
{
	return MeshComponent->GetStaticMesh();
}
