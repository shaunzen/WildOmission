// Copyright Telephone Studios. All Rights Reserved.


#include "FoodItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "Kismet/GameplayStatics.h"

AFoodItem::AFoodItem()
{
	Energy = 0.0f;
	Hydration = 0.0f;
}

void AFoodItem::Primary()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	Super::Primary();

	OwnerCharacter->GetVitalsComponent()->AddHunger(Energy);
	OwnerCharacter->GetVitalsComponent()->AddThirst(Hydration);

	Client_PlayConsumeSound();
	
	OwnerCharacter->GetInventoryComponent()->RemoveHeldItem();
}

uint8 AFoodItem::GetEnergy()
{
	return Energy;
}

uint8 AFoodItem::GetHydration()
{
	return Hydration;
}

void AFoodItem::SetEnergy(uint8 InEnergy)
{
	Energy = InEnergy;
}

void AFoodItem::SetHydration(uint8 InHydration)
{
	Hydration = InHydration;
}

void AFoodItem::Client_PlayConsumeSound_Implementation()
{
	if (ConsumptionSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumptionSound, GetActorLocation());
}