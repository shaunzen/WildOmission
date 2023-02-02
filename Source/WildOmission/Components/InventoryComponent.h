// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "WildOmission/Structs/InventoryItem.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

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
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FInventoryItem Item;
	UPROPERTY()
	int32 Index;
	
	void SetItem(const FInventoryItem& InItem)
	{
		Item = InItem;
	}

	void ClearItem()
	{
		Item.Clear();
	}

	bool IsFull() const
	{
		// this is temporary
		return Item.Quantity >= 10;
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
	
	void Setup(UInventoryWidget* InInventoryWidget);

	// Checks for slot and asks the server to add the item to our contents
	void AddItem(const FName& ItemName, const int32& Quantity);
	void RemoveItem(const FName& ItemName, const int32& Quantity, bool bSpawnInWorld = false);
	void SwapItem();

	// Calls an rpc on the server to spawn a world item
	void SpawnWorldItem(const FName& ItemName, const int32& Quantity = 1);

	// Gets the contents map for this inventory
	FInventoryContents* GetContents();

	// Retrives the data about the item id passed in
	FItem* GetItemData(const FName& ItemName);

	// Gets the widget this inventory is using
	UInventoryWidget* GetWidget();

	void SlotInteraction(const int32& SlotIndex, bool Primary = true);
	void DropSelectedItemInWorld(bool Single = false);
	TArray<FInventorySlot>& GetSlots();
	FInventoryItem* GetSelectedItem();
	bool IsDragging() const;

	FWildOmissionInventorySave Save();
	void Load(const FWildOmissionInventorySave& InInventorySave);

private:
	UPROPERTY(Replicated)
	FInventoryContents Contents;

	UPROPERTY(EditDefaultsOnly)
	uint8 SlotCount;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<FInventorySlot> Slots;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	FInventoryItem SelectedItem;
	bool Dragging;

	//*************************************************
	//	Slot stuff
	//*************************************************
	bool AddItemToSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	bool AddItemToPopulatedSlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd);
	bool AddItemToEmptySlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd);

	bool RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining);
	
	void Drag(const int32& FromSlotIndex, bool bSplit = false); // The Slot We Are Dragging from, which action (take/split)
	void Drop(const int32& ToSlotIndex, bool bSingle = false); // The Destination slot, which action (all/single)


	UFUNCTION(Server, Reliable)
	void Server_AddItem(const FName& ItemName, const int32& Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(const FName& ItemName, const int32& Quantity);
	
	UFUNCTION(Server, Reliable)
	void Server_SpawnWorldItem(const FName& ItemName, const int32& Quantity);

};
