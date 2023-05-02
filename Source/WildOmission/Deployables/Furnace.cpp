// Copyright Telephone Studios. All Rights Reserved.


#include "Furnace.h"
#include "WildOmission/Components/InventoryComponent.h"
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

	DOREPLIFETIME(AFurnace, bTurnedOn);
}

void AFurnace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bTurnedOn)
	{
		SmeltingTick();
	}
}

void AFurnace::Server_ToggleState_Implementation()
{
	bTurnedOn = !bTurnedOn;

	if (bTurnedOn)
	{
		OnTurnedOn();
	}
	else
	{
		// clear any timers
		OnTurnedOff();
	}
}

void AFurnace::OnTurnedOn()
{
	// TODO turn on effects
}

void AFurnace::OnTurnedOff()
{
	// TODO turn off effects
}

void AFurnace::SmeltingTick()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SmeltTimerHandle))
	{
		return;
	}

	int32 AmountOfFuel = GetInventoryComponent()->GetContents()->GetItemQuantity(FName("wood"));
	
	// is there any fuel?
		// turn off if not

	// set timer for five seconds



}

void AFurnace::SmeltItems()
{
	// on timer fire remove wood
	// if something is smeltable remove it
	// and add its smelted form
}