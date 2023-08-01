// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ConsumableItem.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/VitalsComponent.h"
#include "Kismet/GameplayStatics.h"

void AConsumableItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	if (!HasAuthority())
	{
		return;
	}

	UPlayerInventoryComponent* OwnerInventoryComponent = GetOwnerPawn()->FindComponentByClass<UPlayerInventoryComponent>();
	UVitalsComponent* OwnerVitalsComponent = GetOwnerPawn()->FindComponentByClass<UVitalsComponent>();
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
	
	if (Health != -1 && Health != 0)
	{
		OwnerVitalsComponent->AddHealth(Health);
	}
	
	if (Energy != -1 && Energy != 0)
	{
		OwnerVitalsComponent->AddHunger(Energy);
	}
	
	if (Hydration != -1 && Energy != 0)
	{
		OwnerVitalsComponent->AddThirst(Hydration);
	}

	Multi_PlayConsumeSound();

	OwnerInventoryComponent->RemoveHeldItem();
}

void AConsumableItem::Multi_PlayConsumeSound_Implementation()
{
	if (ConsumptionSound == nullptr || GetOwner() == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumptionSound, GetOwner()->GetActorLocation());
}