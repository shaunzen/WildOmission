// Copyright Telephone Studios. All Rights Reserved.

#pragma once

// TODO clean these up
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/InventoryContents.h"
#include "WildOmission/Core/Structs/ItemData.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "InventoryComponent.generated.h"

class UDataTable;
class UInventoryManipulatorComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;
	
	void SetManipulator(UInventoryManipulatorComponent* InventoryManipulator);

	UFUNCTION(BlueprintCallable)
	void AddItem(const FInventoryItem& ItemToAdd);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(const FInventoryItem& ItemToRemove);	

	UFUNCTION(Server, Reliable)
	void Server_SlotInteraction(const int32& SlotIndex, bool Primary = true);

	FItemData* GetItemData(const FName& ItemName);
	FInventoryItem* FindItemWithUniqueID(const uint32& UniqueID);
	FInventorySlot* FindSlotContainingItem(const FName& ItemToFind);	

	FInventoryContents* GetContents();
	FInventorySlot* GetSlot(const int32& SlotIndex);

	// this is a temporary function only for testing
	TArray<FInventorySlot>& GetSlots();
	
	UInventoryManipulatorComponent* GetManipulator() const;

	FWildOmissionInventorySave Save();	
	void Load(const FWildOmissionInventorySave& InInventorySave);

protected:

	UPROPERTY(EditDefaultsOnly)
	uint8 SlotCount;

	UPROPERTY(Replicated, VisibleAnywhere)
	FInventoryContents Contents;

	UPROPERTY(Replicated, ReplicatedUsing = OnInventoryChange, VisibleAnywhere)
	TArray<FInventorySlot> Slots;
	
	UPROPERTY()
	UInventoryManipulatorComponent* Manipulator;

	UFUNCTION()
	virtual void OnInventoryChange();

	virtual void RefreshUI();

private:
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;

	bool AddItemToSlots(const FInventoryItem& ItemToAdd, int32& Remaining);
	bool FindAndAddToPopulatedSlot(const FName& ItemName, const int32& ItemStackSize, int32& QuantityToAdd);
	bool FindAndAddToEmptySlot(const FName& ItemName, const int32& ItemStackSize, const TArray<FItemStat>& Stats, int32& QuantityToAdd);
	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void DragAll(const int32& FromSlotIndex);
	void DragSplit(const int32& FromSlotIndex);
	void DropAll(const int32& ToSlotIndex);
	void DropSingle(const int32& ToSlotIndex);

	bool WithinStackSize(const FInventoryItem& Item, const int32& AmountToAdd);
};
