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
static UDataTable* UIColorsTable = nullptr;

UWildOmissionStatics::UWildOmissionStatics()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_Items"));
	if (ItemDataTableBlueprint.Succeeded())
	{
		ItemDataTable = ItemDataTableBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> UIColorsTableBlueprint(TEXT("/Game/WildOmission/UI/Custom/DT_UIColors"));
	if (UIColorsTableBlueprint.Succeeded())
	{
		UIColorsTable = UIColorsTableBlueprint.Object;
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

FUIColor* UWildOmissionStatics::GetUIColor(const FName& ColorName)
{
	if (UIColorsTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("UI Color Context"));

	return UIColorsTable->FindRow<FUIColor>(ColorName, ContextString, true);
}

FLinearColor UWildOmissionStatics::GetHighlightedColor(FUIColor* Color)
{
	return FLinearColor(Color->Default.R + Color->HighlightedOffset, Color->Default.G + Color->HighlightedOffset, Color->Default.B + Color->HighlightedOffset, 1.0f);
}

FLinearColor UWildOmissionStatics::GetSelectedColor(FUIColor* Color)
{
	return FLinearColor(Color->Default.R + Color->SelectedOffset, Color->Default.G + Color->SelectedOffset, Color->Default.B + Color->SelectedOffset, 1.0f);
}

FVector UWildOmissionStatics::GetHostLocationInWorld(UWorld* WorldContextObject)
{
	APlayerController* HostPlayerController = WorldContextObject->GetFirstPlayerController();
	if (HostPlayerController == nullptr)
	{
		return FVector::ZeroVector;
	}

	APawn* HostPawn = HostPlayerController->GetPawn();
	if (HostPawn == nullptr)
	{
		return FVector::ZeroVector;
	}

	return HostPawn->GetActorLocation();
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