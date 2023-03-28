// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItemSaveHandlerComponent.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Items/WorldItem.h"

// Sets default values for this component's properties
UWorldItemSaveHandlerComponent::UWorldItemSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UWorldItemSaveHandlerComponent::Save(TArray<FWorldItemSave>& OutSaves)
{
	OutSaves.Empty();

	TArray<AActor*> WorldItemActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldItem::StaticClass(), WorldItemActors);
	
	for (AActor* WorldItemActor : WorldItemActors)
	{
		AWorldItem* WorldItem = Cast<AWorldItem>(WorldItemActor);
		if (WorldItem == nullptr || WorldItem->IgnoredInSave())
		{
			return;
		}

		FWorldItemSave WorldItemSave;
		WorldItemSave.WorldLocation = WorldItem->GetActorLocation();
		WorldItemSave.Item = WorldItem->GetItem();

		OutSaves.Add(WorldItemSave);
	}
}

void UWorldItemSaveHandlerComponent::Load(const TArray<FWorldItemSave>& InSaves)
{
	for (const FWorldItemSave& WorldItemSave : InSaves)
	{
		AWorldItem* WorldItem = GetWorld()->SpawnActor<AWorldItem>();
		WorldItem->SetActorLocation(WorldItemSave.WorldLocation);
		WorldItem->SetItem(WorldItemSave.Item);
	}
}