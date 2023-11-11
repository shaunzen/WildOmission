// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/ToolCupboard.h"

static TArray<AToolCupboard*> AllToolCupboards;

AToolCupboard::AToolCupboard()
{
	AuthorizedPlayers = TArray<FString>();
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
