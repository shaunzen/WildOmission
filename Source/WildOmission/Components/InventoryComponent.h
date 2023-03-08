// Copyright Telephone Studios. All Rights Reserved.

#pragma once

// TODO clean these up
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/InventoryContents.h"
#include "WildOmission/Core/Structs/ItemData.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "InventoryComponent.generated.h"

class UInventoryManipulatorComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	void SetManipulator(UInventoryManipulatorComponent* InventoryManipulator);

	//this could benifit from InventoryItem
	void AddItem(const FInventoryItem& ItemToAdd);

	void RemoveItem(const FInventoryItem& ItemToRemove, bool bDropInWorld = false);
	
	UFUNCTION(Server, Reliable)
	void Server_SlotInteraction(const int32& SlotIndex, bool Primary = true);
	
	FInventoryContents* GetContents();

	TArray<FInventorySlot>& GetSlots();

	FInventoryItem* FindItemWithUniqueID(const uint32& UniqueID);

	FInventorySlot* FindSlotContainingItem(const FName& ItemToFind);

	FInventorySlot* FindSlotContainingItemWithUniqueID(const uint32& UniqueID);
	
	FItemData* GetItemData(const FName& ItemName);
	
	UInventoryManipulatorComponent* GetManipulator();

	FWildOmissionInventorySave Save();
	
	void Load(const FWildOmissionInventorySave& InInventorySave);

protected:

	UPROPERTY(EditDefaultsOnly)
	uint8 SlotCount;

	UPROPERTY(Replicated)
	FInventoryContents Contents;

	UPROPERTY(Replicated, ReplicatedUsing = RefreshUI)
	TArray<FInventorySlot> Slots;
	
	UPROPERTY()
	UInventoryManipulatorComponent* Manipulator;

	UFUNCTION()
	virtual void RefreshUI();

	virtual void OnInventoryChange();

private:

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDataTable;

	bool AddItemToSlots(const FInventoryItem& ItemToAdd, int32& Remaining);
	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void DragAll(const int32& FromSlotIndex);
	void DragSplit(const int32& FromSlotIndex);

	void DropAll(const int32& ToSlotIndex);
	void DropSingle(const int32& ToSlotIndex);

	bool FindAndAddToPopulatedSlot(const FName& ItemName, FItemData* ItemData, int32& QuantityToAdd);
	bool FindAndAddToEmptySlot(const FName& ItemName, FItemData* ItemData, const TArray<FItemStat>& Stats, const uint32& ItemUniqueID, int32& QuantityToAdd);

};
