// Copyright Telephone Studios. All Rights Reserved.


#include "Furnace.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

AFurnace::AFurnace()
{
	bCanSpawnOnGround = true;
	bCanSpawnOnFloor = true;
	bCanSpawnOnWall = false;
	bFollowsSurfaceNormal = true;

	SmeltTimeInSeconds = 5.0f;

	ContainerName = FString("Furnace");

	Light = CreateDefaultSubobject<UPointLightComponent>(FName("Light"));
	Light->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AFurnace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFurnace, bTurnedOn);
}

void AFurnace::BeginPlay()
{
	Super::BeginPlay();

	OnRep_TurnedOn();
}

void AFurnace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bTurnedOn)
	{
		SmeltingTick();
	}
}

void AFurnace::Server_ToggleState_Implementation(bool bState)
{
	bTurnedOn = bState;

	if(bTurnedOn == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(SmeltTimerHandle);
	}

	OnRep_TurnedOn();
}

bool AFurnace::IsTurnedOn() const
{
	return bTurnedOn;
}

void AFurnace::OnTurnedOn()
{
	// TODO turn on effects
	Light->SetVisibility(true);
	AudioComponent->Play();
}

void AFurnace::OnTurnedOff()
{
	// TODO turn off effects
	Light->SetVisibility(false);
	AudioComponent->Stop();
}

void AFurnace::OnRep_TurnedOn()
{
	if (bTurnedOn)
	{
		OnTurnedOn();
	}
	else
	{
		OnTurnedOff();
	}
}

void AFurnace::SmeltingTick()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SmeltTimerHandle))
	{
		return;
	}

	int32 AmountOfFuel = GetInventoryComponent()->GetContents()->GetItemQuantity(FName("wood"));
	if (AmountOfFuel < 2)
	{
		Server_ToggleState(false);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(SmeltTimerHandle, this, &AFurnace::SmeltItems, SmeltTimeInSeconds, false);
}

void AFurnace::SmeltItems()
{
	int32 AmountOfFuel = GetInventoryComponent()->GetContents()->GetItemQuantity(FName("wood"));
	if (AmountOfFuel < 2)
	{
		Server_ToggleState(false);
		return;
	}

	FInventoryItem FuelToRemove;
	FuelToRemove.Name = FName("wood");
	FuelToRemove.Quantity = 2;
	GetInventoryComponent()->RemoveItem(FuelToRemove);

	int32 AmountOfMetalOre = GetInventoryComponent()->GetContents()->GetItemQuantity(FName("metal_ore"));
	if (AmountOfMetalOre > 0)
	{
		FInventoryItem MetalOreToRemove;
		MetalOreToRemove.Name = FName("metal_ore");
		MetalOreToRemove.Quantity = 1;
		GetInventoryComponent()->RemoveItem(MetalOreToRemove);

		FInventoryItem SmeltedMetalToAdd;
		SmeltedMetalToAdd.Name = FName("metal");
		SmeltedMetalToAdd.Quantity = 1;
		GetInventoryComponent()->AddItem(SmeltedMetalToAdd, this);
	}
}