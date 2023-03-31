// Copyright Telephone Studios. All Rights Reserved.


#include "ConsumableItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "Kismet/GameplayStatics.h"

void AConsumableItem::Primary()
{
	Super::Primary();
	UPlayerInventoryComponent* OwnerInventoryComponent = GetOwnerCharacter()->FindComponentByClass<UPlayerInventoryComponent>();
	UVitalsComponent* OwnerVitalsComponent = GetOwnerCharacter()->FindComponentByClass<UVitalsComponent>();
	if (OwnerInventoryComponent == nullptr || OwnerVitalsComponent == nullptr)
	{
		return;
	}

	FItemData* ItemData = UInventoryComponent::GetItemData(ItemName);
	if (ItemData == nullptr)
	{
		return;
	}

	int32 Health = ItemData->GetStat(FName("Health"));
	int32 Energy = ItemData->GetStat(FName("Energy"));
	int32 Hydration = ItemData->GetStat(FName("Hydration"));
	
	if (Health > 0)
	{
		OwnerVitalsComponent->AddHealth(Health);
	}
	
	if (Energy > 0)
	{
		OwnerVitalsComponent->AddHunger(Energy);
	}
	
	if (Hydration > 0)
	{
		OwnerVitalsComponent->AddThirst(Hydration);
	}

	Client_PlayConsumeSound();

	OwnerInventoryComponent->RemoveHeldItem();
}

void AConsumableItem::Client_PlayConsumeSound_Implementation()
{
	if (ConsumptionSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumptionSound, GetActorLocation());
}