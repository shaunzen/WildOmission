// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UPlayerInventoryComponent();
	
	virtual void BeginPlay() override;

	//**************************************************************
	// General Management
	//**************************************************************
	
	void RefreshPlayerEquip(FInventorySlot& SelectedSlot);

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

	UPROPERTY()
	class AWildOmissionCharacter* OwnerCharacter;

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
