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

	int32 Health = ItemData->GetStat(TEXT("Health"));
	int32 Energy = ItemData->GetStat(TEXT("Energy"));
	int32 Hydration = ItemData->GetStat(TEXT("Hydration"));
	
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
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumptionSound, GetOwner()->GetActorLocation());
}