// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "WildOmission/Items/EquipableItem.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WildOmission/Core/Structs/InventorySlot.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "InventoryComponent.generated.h"

class UInventoryManipulatorComponent;

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEquipableItem> EquipItemClass;

	FItem()
	{
		DisplayName = FName(TEXT("Item"));
		Description = FString(TEXT("This is an Item."));
		Thumbnail = nullptr;
		Mesh = nullptr;
		StackSize = 1000;
	}
};

USTRUCT()
struct FInventoryContents
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryItem> Contents;

	// Returns the amount of a given item in the inventory, will return 0 if item isn't present.
	int32 GetItemQuantity(const FName& ItemName)
	{
		int32 Index = GetItemIndex(ItemName);
		if (Index == INDEX_NONE)
		{
			return 0;
		}
		return Contents[Index].Quantity;
	}

	// Returns true if the specified item is present
	bool HasItem(const FName& ItemName)
	{
		return Contents.FindByPredicate([&ItemName](const FInventoryItem& ItemData) {
			return ItemData.CompareNames(ItemData, ItemName);
		}) != nullptr;
	}
	
	// Will add the given item and quantity to the list, if item is already present the quantity will be added to the existing.
	void AddItem(const FName& ItemName, const int32& QuantityToAdd)
	{
		if (HasItem(ItemName))
		{
			int32 ItemIndex = GetItemIndex(ItemName);
			Contents[ItemIndex].Quantity += QuantityToAdd;
		}
		else
		{
			FInventoryItem NewItem;
			NewItem.Name = ItemName;
			NewItem.Quantity = QuantityToAdd;
			Contents.Add(NewItem);
		}
	}

	void RemoveItem(const FName& ItemName, const int32& QuantityToRemove)
	{
		if (!HasItem(ItemName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot remove %s, item does not exist in inventory."), *ItemName.ToString());
			return;
		}

		int32 ItemIndex = GetItemIndex(ItemName);
		Contents[ItemIndex].Quantity -= QuantityToRemove;

		if (Contents[ItemIndex].Quantity <= 0)
		{
			Contents.RemoveAt(ItemIndex);
		}
	}

private:

	// Returns the index in the contents list of the given item
	int32 GetItemIndex(const FName& ItemName)
	{
		return Contents.IndexOfByPredicate([&ItemName](const FInventoryItem& ItemData) {
			return ItemData.CompareNames(ItemData, ItemName);
		});
	}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	void Setup(UInventoryManipulatorComponent* InventoryManipulator);

	//**************************************************************
	// General Management
	//**************************************************************

	void AddItem(const FName& ItemName, const int32& Quantity);
	
	void RemoveItem(const FName& ItemName, const int32& Quantity, bool bSpawnInWorld = false);

	//**************************************************************
	// User Interaction
	//**************************************************************
	
	void SlotInteraction(const int32& SlotIndex, bool Primary = true);

	//**************************************************************
	// Getters
	//**************************************************************
	
	// Gets the contents map for this inventory
	FInventoryContents* GetContents();

	// Returns a reference to the inventory slots
	TArray<FInventorySlot>& GetSlots();
	
	// Retrives the data about the item id passed in
	FItem* GetItemData(const FName& ItemName);
	
	UInventoryManipulatorComponent* GetManipulator();

	//**************************************************************
	// Save Load
	//**************************************************************

	FWildOmissionInventorySave Save();
	
	void Load(const FWildOmissionInventorySave& InInventorySave);

protected:

	UPROPERTY(Replicated)
	FInventoryContents Contents;

	UPROPERTY(EditDefaultsOnly)
	uint8 SlotCount;

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = RefreshUI)
	TArray<FInventorySlot> Slots;
	
	UPROPERTY()
	UInventoryManipulatorComponent* Manipulator;

	UFUNCTION()
	virtual void RefreshUI();
	
	virtual void OnInventoryChange();

private:

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;

	//**************************************************************
	// Slot Functions
	//**************************************************************

	bool AddItemToSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void DragAll(const int32& FromSlotIndex);
	void DragSplit(const int32& FromSlotIndex);

	void DropAll(const int32& ToSlotIndex);
	void DropSingle(const int32& ToSlotIndex);

	bool FindAndAddToPopulatedSlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd);
	bool FindAndAddToEmptySlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd);

	//**************************************************************
	// RPC
	//**************************************************************

	/*General Item Mananagement*/
	UFUNCTION(Server, Reliable)
	void Server_AddItemToContents(const FName& ItemName, const int32& Quantity);

	UFUNCTION(Server, Reliable)
	void Server_AddItem(const FName& ItemName, const int32& Quantity);


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItemFromContents(const FName& ItemName, const int32& Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(const FName& ItemName, const int32& Quantity);

	/*Drag and drop*/
	UFUNCTION(Server, Reliable)
	void Server_DragAll(const int32& FromSlotIndex);
	
	UFUNCTION(Server, Reliable)
	void Server_DragSplit(const int32& FromSlotIndex);

	UFUNCTION(Server, Reliable)
	void Server_DropAll(const int32& ToSlotIndex);
	
	UFUNCTION(Server, Reliable)
	void Server_DropSingle(const int32& ToSlotIndex);

};
