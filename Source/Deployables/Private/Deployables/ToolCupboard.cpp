// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/ToolCupboard.h"

static TArray<AToolCupboard*> AllToolCupboards;

AToolCupboard::AToolCupboard()
{
	Range = 50000.0f;
	AuthorizedPlayers = TArray<FString>();
}

void AToolCupboard::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		return;
	}

	// TODO open menu
}

FString AToolCupboard::PromptText()
{
	return TEXT("open authorization menu");
}

void AToolCupboard::AuthorizePlayer(const FString& PlayerUniqueID)
{
	AuthorizedPlayers.Add(PlayerUniqueID);
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

	AllToolCupboards.Add(this);
}

void AToolCupboard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AllToolCupboards.Remove(this);
}
