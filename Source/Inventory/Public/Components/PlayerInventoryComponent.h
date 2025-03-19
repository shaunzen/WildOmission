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

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayItemBreakSound();

	//**************************************************************
	// General Management
	//**************************************************************
	
	FOnToolbarSlotSelectionChange OnToolbarSlotSelectionChange;

	//**************************************************************
	// User Interaction
	//**************************************************************

	// Toolbar slot selection is processed locally, and then sent to the server via an RPC
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

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ItemBreakSound;

	//**************************************************************
	// Slot Functions
	//**************************************************************

	UFUNCTION()
	void RefreshToolbarSelectionState();
	
	bool IsToolbarSlotSelectionValid() const;

	UFUNCTION()
	bool UseAlphaToolbarScroll() const;

	//**************************************************************
	// RPC
	//**************************************************************

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetToolbarSelectionIndex(int8 SelectionIndex);

};
