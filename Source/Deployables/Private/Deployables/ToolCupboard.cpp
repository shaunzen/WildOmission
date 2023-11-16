// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/ToolCupboard.h"
#include "Components/LockComponent.h"
#include "Locks/Lock.h"
#include "Components/BuilderComponent.h"
#include "Net/UnrealNetwork.h"

static TArray<AToolCupboard*> AllToolCupboards;

AToolCupboard::AToolCupboard()
{
	SetNetDormancy(ENetDormancy::DORM_DormantAll);

	LockComponent = CreateDefaultSubobject<ULockComponent>(TEXT("LockComponent"));
	LockComponent->SetupAttachment(MeshComponent);

	Range = 5000.0f;
	AuthorizedPlayers = TArray<FString>();
}

void AToolCupboard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToolCupboard, AuthorizedPlayers);
}

void AToolCupboard::OnSpawn()
{
	Super::OnSpawn();
	AllToolCupboards.Add(this);
}

void AToolCupboard::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		return;
	}

	if (IsValid(LockComponent->GetLock()) && !LockComponent->GetLock()->IsAuthorized(Cast<APawn>(Interactor)) && LockComponent->GetLock()->IsLocked())
	{
		return;
	}

	UBuilderComponent* InteractorBuilderComponent = Interactor->FindComponentByClass<UBuilderComponent>();
	if (InteractorBuilderComponent == nullptr)
	{
		return;
	}

	InteractorBuilderComponent->OpenToolCupboardMenu(this);
}

FString AToolCupboard::PromptText()
{
	return TEXT("open authorization menu");
}

void AToolCupboard::AuthorizePlayer(const FString& PlayerUniqueID)
{
	AuthorizedPlayers.Add(PlayerUniqueID);
	FlushNetDormancy();
}

void AToolCupboard::DeauthorizePlayer(const FString& PlayerUniqueID)
{
	AuthorizedPlayers.Remove(PlayerUniqueID);
	FlushNetDormancy();
}

bool AToolCupboard::IsPlayerAuthorized(const FString& PlayerUniqueID) const
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

void AToolCupboard::ClearAuthorizedPlayers()
{
	AuthorizedPlayers.Empty();
	FlushNetDormancy();
}

bool AToolCupboard::IsWithinRange(const FVector& LocationToTest) const
{
	const float Distance = FVector::Distance(this->GetActorLocation(), LocationToTest);
	if (Distance <= Range)
	{
		return true;
	}
	
	return false;
}

TArray<AToolCupboard*> AToolCupboard::GetAllToolCupboards()
{
	return AllToolCupboards;
}

void AToolCupboard::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		AllToolCupboards.Add(this);
	}
}

void AToolCupboard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AllToolCupboards.Remove(this);
}

void AToolCupboard::OnLoadComplete_Implementation()
{
	AllToolCupboards.Add(this);
}
