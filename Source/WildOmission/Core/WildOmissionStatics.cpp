// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionStatics.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WildOmission/Items/WorldItem.h"
#include "WildOmission/Core/GameModes/WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Core/SaveSystem/WorldGenerationHandlerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

static UDataTable* ItemDataTable = nullptr;

UWildOmissionStatics::UWildOmissionStatics()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Game/WildOmission/Core/DataTables/DT_Items"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
	}
}

FItemData* UWildOmissionStatics::GetItemData(const FName& ItemName)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Item Data Context"));

	return ItemDataTable->FindRow<FItemData>(ItemName, ContextString, true);
}

void UWildOmissionStatics::SpawnWorldItem(UWorld* WorldContextObject, const FInventoryItem& ItemToSpawn, AActor* SpawningActor)
{
	if (WorldContextObject == nullptr || SpawningActor == nullptr)
	{
		return;
	}

	// Get the data for this item
	FItemData* ItemData = UWildOmissionStatics::GetItemData(ItemToSpawn.Name);

	// Spawn a world item actor
	AWorldItem* WorldItem = WorldContextObject->SpawnActor<AWorldItem>();
	if (ItemData == nullptr || WorldItem == nullptr)
	{
		return;
	}

	WorldItem->SetOwner(SpawningActor);

	FVector SpawnLocation;
	FVector PhysicsImpulse;

	SpawnLocation = SpawningActor->GetActorLocation();
	PhysicsImpulse = SpawningActor->GetActorForwardVector() * 5000.0f;

	// Update world items properties
	WorldItem->SetActorLocation(SpawnLocation);
	WorldItem->SetItem(ItemToSpawn);
	WorldItem->AddImpulse(PhysicsImpulse);
}

void UWildOmissionStatics::GetWorldSize(UWorld* WorldContextObject, FVector2D& OutWorldSize)
{
	if (WorldContextObject == nullptr)
	{
		return;
	}
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(WorldContextObject->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}
	ASaveHandler* SaveHandler = GameMode->GetSaveHandler();
	if (SaveHandler == nullptr)
	{
		return;
	}

	OutWorldSize = SaveHandler->GetWorldGenerationHandler()->GetWorldSizeMeters();
}