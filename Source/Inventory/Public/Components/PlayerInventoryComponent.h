// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnToolbarSlotSelectionChange, const int8&, NewSlotIndex, const FInventorySlot&, NewSlot);

UCLASS()
class INVENTORY_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UPlayerInventoryComponent();
	
	virtual void BeginPlay() override;

	//**************************************************************
	// General Management
	//**************************************************************
	
	void RefreshPlayerEquip(FInventorySlot& SelectedSlot);
	FOnToolbarSlotSelectionChange OnToolbarSlotSelectionChange;

	//**************************************************************
	// User Interaction
	//**************************************************************

	void IncrementToolbarSelection();
	void DecrementToolbarSelection();

	void SetToolbarSelectionIndex(int8 SelectionIndex);

	void RemoveHeldItem();

	//**************************************************************
	// Getters
	//**************************************************************

	int8 GetToolbarSelectionIndex();
	
protected:

	virtual void OnRep_ServerState() override;

private:

	UPROPERTY()
	int8 ToolbarSelectionIndex;

	//**************************************************************
	// Slot Functions
	//**************************************************************

	UFUNCTION()
	void RefreshToolbarSelectionState();
	
	bool IsToolbarSlotSelectionValid() const;

	//**************************************************************
	// RPC
	//**************************************************************

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetToolbarSelectionIndex(int8 SelectionIndex);

};
