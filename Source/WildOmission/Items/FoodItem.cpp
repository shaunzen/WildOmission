// Copyright Telephone Studios. All Rights Reserved.


#include "FoodItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "Kismet/GameplayStatics.h"

void AFoodItem::Primary()
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

	int32 Energy = ItemData->GetStat(FName("Energy"));
	int32 Hydration = ItemData->GetStat(FName("Hydration"));
	
	OwnerVitalsComponent->AddHunger(Energy);
	OwnerVitalsComponent->AddThirst(Hydration);

	Client_PlayConsumeSound();
	
	OwnerInventoryComponent->RemoveHeldItem();
}

void AFoodItem::Client_PlayConsumeSound_Implementation()
{
	if (ConsumptionSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumptionSound, GetActorLocation());
}