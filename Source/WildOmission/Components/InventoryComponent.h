// Copyright Telephone Studios. All Rights Reserved.

#pragma once

// TODO clean these up
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/InventoryContents.h"
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

	//**************************************************************
	// General Management
	//**************************************************************

	void AddItem(const FName& ItemName, const int32& Quantity, const TArray<FItemStat>& Stats);

	void RemoveItem(const FName& ItemName, const int32& Quantity, bool bDropInWorld = false);

	//**************************************************************
	// User Interaction
	//**************************************************************
	
	UFUNCTION(Server, Reliable)
	void Server_SlotInteraction(const int32& SlotIndex, bool Primary = true);

	//**************************************************************
	// Getters
	//**************************************************************
	
	// Gets the contents map for this inventory
	FInventoryContents* GetContents();

	// Returns a reference to the inventory slots
	TArray<FInventorySlot>& GetSlots();

	// Returns a poiter to the item with the specified unique id
	FInventoryItem* FindItemWithUniqueID(const uint32& UniqueID);

	// Returns a pointer to slot containing the specified item
	FInventorySlot* FindSlotContainingItem(const FName& ItemToFind);

	// Returns a poiter to the slot containing an item with the specified unique id
	FInventorySlot* FindSlotContainingItemWithUniqueID(const uint32& UniqueID);
	
	// Retrives the data about the item id passed in
	struct FItemData* GetItemData(const FName& ItemName);
	
	UInventoryManipulatorComponent* GetManipulator();

	//**************************************************************
	// Save Load
	//**************************************************************

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

	bool AddItemToSlots(const FName& ItemName, const int32& Quantity, const TArray<FItemStat>& Stats, int32& Remaining);
	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void DragAll(const int32& FromSlotIndex);
	void DragSplit(const int32& FromSlotIndex);

	void DropAll(const int32& ToSlotIndex);
	void DropSingle(const int32& ToSlotIndex);

	bool FindAndAddToPopulatedSlot(const FName& ItemName, FItemData* ItemData, int32& QuantityToAdd);
	bool FindAndAddToEmptySlot(const FName& ItemName, FItemData* ItemData, const TArray<FItemStat>& Stats, const uint32& ItemUniqueID, int32& QuantityToAdd);

};
