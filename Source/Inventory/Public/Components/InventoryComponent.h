// Copyright Telephone Studios. All Rights Reserved.

#pragma once

// TODO clean these up
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/InventoryContents.h"
#include "Structs/ItemData.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "InventoryComponent.generated.h"

class UInventoryManipulatorComponent;

USTRUCT()
struct FInventorySlotInteraction
{
	GENERATED_BODY()

	UPROPERTY()
	float Time = 0;

	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	UInventoryManipulatorComponent* Manipulator;

	UPROPERTY()
	bool Primary = true;

};

USTRUCT()
struct FInventoryItemUpdate
{
	GENERATED_BODY()

	UPROPERTY()
	float Time = 0;

	UPROPERTY()
	bool Addition = true;

	UPROPERTY()
	FInventoryItem Item;
};

USTRUCT()
struct FInventoryState
{
	GENERATED_BODY()

	UPROPERTY()
	FInventorySlotInteraction LastInteraction;

	UPROPERTY()
	FInventoryContents Contents;

	UPROPERTY()
	TArray<FInventorySlot> Slots;
	
	UPROPERTY()
	TArray<FInventoryItemUpdate> Updates;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUpdateSignature, const FInventoryItemUpdate&, ItemUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UInventoryComponent : public UActorComponent, public ISavableObjectInterface
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void AddItem(const FInventoryItem& ItemToAdd, AActor* ActorToSpawnDropedItems = nullptr);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(const FInventoryItem& ItemToRemove);	

	void SlotInteraction(const int32& SlotIndex, UInventoryManipulatorComponent* Manipulator, bool Primary = true);
	
	static FItemData* GetItemData(const FName& ItemID);
	static void SpawnWorldItem(UWorld* WorldContextObject, const struct FInventoryItem& ItemToSpawn, AActor* SpawningActor);

	FInventoryUpdateSignature OnUpdate;
	FOnInventoryItemUpdateSignature OnItemUpdate;

	FInventoryItem* FindItemWithUniqueID(const uint32& UniqueID);
	FInventorySlot* FindSlotContainingItem(const FName& ItemToFind);	

	FInventoryContents* GetContents();
	FInventorySlot* GetSlot(const int32& SlotIndex);
	uint8 GetSlotCount() const;
	TArray<FInventorySlot>& GetSlots();
	
	void SetSlotCount(const uint8& InSlotCount);

	UFUNCTION(BlueprintCallable)
	bool WasLoadedFromSave() const;

	FWildOmissionInventorySave Save();	
	void Load(const FWildOmissionInventorySave& InInventorySave);

protected:

	UPROPERTY(EditDefaultsOnly)
	uint8 SlotCount;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FInventoryContents Contents;

	UPROPERTY(VisibleAnywhere, SaveGame)
	TArray<FInventorySlot> Slots;

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRep_ServerState)
	FInventoryState ServerState;

	UFUNCTION()
	virtual void OnRep_ServerState();

	UFUNCTION()
	void BroadcastInventoryUpdate();

	UFUNCTION()
	void BroadcastItemUpdate(const FInventoryItemUpdate& ItemUpdate);

private:
	bool AddItemToSlots(const FInventoryItem& ItemToAdd, int32& Remaining);
	bool FindAndAddToPopulatedSlot(const FName& ItemName, const int32& ItemStackSize, int32& QuantityToAdd);
	bool FindAndAddToEmptySlot(const FName& ItemName, const int32& ItemStackSize, const TArray<FItemStat>& Stats, int32& QuantityToAdd);
	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void DragAll(const int32& FromSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify);
	void DragSplit(const int32& FromSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify);
	void DropAll(const int32& ToSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify);
	void DropSingle(const int32& ToSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify);

	bool WithinStackSize(const FInventoryItem& Item, const int32& AmountToAdd);

	TArray<FInventorySlotInteraction> UnacknowalgedInteractions;

	FInventoryItemUpdate LastClientAcknowlagedItemUpdate;

	void ClearAcknowlagedInteractions(const FInventorySlotInteraction& LastInteraction);
	void GetUnacknowlagedUpdates(TArray<FInventoryItemUpdate>& OutUpdatesList);

	void BroadcastAllUpdateNotifications(const TArray<FInventoryItemUpdate>& UpdatesList);

	bool LoadedFromSave;

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

	UFUNCTION(Server, Reliable)
	void Server_SlotInteraction(FInventorySlotInteraction Interaction);

};
