// Copyright Telephone Studios. All Rights Reserved.


#include "Furnace.h"
#include "Net/UnrealNetwork.h"

AFurnace::AFurnace()
{
	bCanSpawnOnGround = true;
	bCanSpawnOnFloor = true;
	bCanSpawnOnWall = false;
	bFollowsSurfaceNormal = true;
}

void AFurnace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFurnace, TurnedOn);
}

void AFurnace::Server_ToggleState_Implementation()
{
	TurnedOn = !TurnedOn;
}