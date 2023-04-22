// Copyright Telephone Studios. All Rights Reserved.

#pragma once

// TODO clean these up
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/InventoryContents.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "InventoryComponent.generated.h"

class UInventoryManipulatorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void AddItem(const FInventoryItem& ItemToAdd, UInventoryManipulatorComponent* Manipulator);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(const FInventoryItem& ItemToRemove);	

	UFUNCTION(Server, Reliable)
	void Server_SlotInteraction(const int32& SlotIndex, UInventoryManipulatorComponent* Manipulator, bool Primary = true);

	FInventoryUpdateSignature OnUpdate;
	
	FInventoryItem* FindItemWithUniqueID(const uint32& UniqueID);
	FInventorySlot* FindSlotContainingItem(const FName& ItemToFind);	

	FInventoryContents* GetContents();
	FInventorySlot* GetSlot(const int32& SlotIndex);
	uint8 GetSlotCount() const;

	TArray<FInventorySlot>& GetSlots();
	
	FWildOmissionInventorySave Save();	
	void Load(const FWildOmissionInventorySave& InInventorySave);

protected:

	UPROPERTY(EditDefaultsOnly)
	uint8 SlotCount;

	UPROPERTY(Replicated, VisibleAnywhere)
	FInventoryContents Contents;

	UPROPERTY(Replicated, ReplicatedUsing = BroadcastInventoryUpdate, VisibleAnywhere)
	TArray<FInventorySlot> Slots;

	UFUNCTION()
	void BroadcastInventoryUpdate();

	bool LoadedFromSave;

private:
	bool AddItemToSlots(const FInventoryItem& ItemToAdd, int32& Remaining);
	bool FindAndAddToPopulatedSlot(const FName& ItemName, const int32& ItemStackSize, int32& QuantityToAdd);
	bool FindAndAddToEmptySlot(const FName& ItemName, const int32& ItemStackSize, const TArray<FItemStat>& Stats, int32& QuantityToAdd);
	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void DragAll(const int32& FromSlotIndex, UInventoryManipulatorComponent* Manipulator);
	void DragSplit(const int32& FromSlotIndex, UInventoryManipulatorComponent* Manipulator);
	void DropAll(const int32& ToSlotIndex, UInventoryManipulatorComponent* Manipulator);
	void DropSingle(const int32& ToSlotIndex, UInventoryManipulatorComponent* Manipulator);

	bool WithinStackSize(const FInventoryItem& Item, const int32& AmountToAdd);

};
