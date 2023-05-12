// Copyright Telephone Studios. All Rights Reserved.

#pragma once

// TODO clean these up
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/InventoryContents.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "InventoryComponent.generated.h"

class UInventoryManipulatorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);

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
struct FInventoryState
{
	GENERATED_BODY()

	UPROPERTY()
	FInventorySlotInteraction LastInteraction;

	UPROPERTY()
	FInventoryContents Contents;

	UPROPERTY()
	TArray<FInventorySlot> Slots;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent, public ISavableObjectInterface
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

	bool LoadedFromSave;

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

	void ClearAcknowlagedInteractions(const FInventorySlotInteraction& LastInteraction);

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

	UFUNCTION(Server, Reliable)
	void Server_SlotInteraction(FInventorySlotInteraction Interaction);

};
