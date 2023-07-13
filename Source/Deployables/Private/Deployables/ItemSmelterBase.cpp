// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/ItemSmelterBase.h"
#include "Components/InventoryComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

AItemSmelterBase::AItemSmelterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanSpawnOnGround = true;
	bCanSpawnOnFloor = true;
	bCanSpawnOnWall = false;
	bFollowsSurfaceNormal = true;

	SmeltTimeSeconds = 5.0f;
	ContainerName = TEXT("Item Smelter");

	GetInventoryComponent()->SetSlotCount(6);

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	LightComponent->SetupAttachment(MeshComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(MeshComponent);
}

void AItemSmelterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemSmelterBase, bTurnedOn);
}

void AItemSmelterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() || !IsTurnedOn())
	{
		return;
	}

	SmeltingTick();
}

bool AItemSmelterBase::IsTurnedOn() const
{
	return bTurnedOn;
}

void AItemSmelterBase::BeginPlay()
{
	Super::BeginPlay();

	OnRep_TurnedOn();
}

void AItemSmelterBase::SmeltingTick()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SmeltTimerHandle))
	{
		return;
	}

	int32 AmountOfFuel = GetInventoryComponent()->GetContents()->GetItemQuantity(FuelSource.Name);
	if (AmountOfFuel < FuelSource.Quantity)
	{
		Server_ToggleState(false);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(SmeltTimerHandle, this, &AItemSmelterBase::OnSmelt, SmeltTimeSeconds, false);
}

void AItemSmelterBase::OnSmelt()
{
	int32 AmountOfFuel = GetInventoryComponent()->GetContents()->GetItemQuantity(FuelSource.Name);
	
	if (AmountOfFuel < FuelSource.Quantity)
	{
		Server_ToggleState(false);
		return;
	}

	GetInventoryComponent()->RemoveItem(FuelSource);

	FindAndSmeltAllSmeltables();

	FlushNetDormancy();
}

void AItemSmelterBase::OnTurnedOn()
{
	MeshComponent->SetCustomPrimitiveDataFloat(0, 1.0f);
	LightComponent->SetVisibility(true);
	AudioComponent->Play();
}

void AItemSmelterBase::OnTurnedOff()
{
	MeshComponent->SetCustomPrimitiveDataFloat(0, 0.0f);
	LightComponent->SetVisibility(false);
	AudioComponent->Stop();
}

void AItemSmelterBase::FindAndSmeltAllSmeltables()
{
	for (const FSmeltResult& Smeltable : Smeltables)
	{
		int32 AmountOfSmeltable = GetInventoryComponent()->GetContents()->GetItemQuantity(Smeltable.RawItem.Name);
		if (AmountOfSmeltable < Smeltable.RawItem.Quantity)
		{
			continue;
		}

		GetInventoryComponent()->RemoveItem(Smeltable.RawItem);
		GetInventoryComponent()->AddItem(Smeltable.SmeltedItem, this);
	}
}

void AItemSmelterBase::OnRep_TurnedOn()
{
	IsTurnedOn() ? OnTurnedOn() : OnTurnedOff();
}

void AItemSmelterBase::OnLoadComplete_Implementation()
{
	OnRep_TurnedOn();
}

void AItemSmelterBase::Server_ToggleState_Implementation(bool NewState)
{
	bTurnedOn = NewState;

	if (bTurnedOn == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(SmeltTimerHandle);
	}

	OnRep_TurnedOn();
}