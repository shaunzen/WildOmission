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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//**************************************************************
	// General Management
	//**************************************************************
	
	void RefreshPlayerEquip(FInventorySlot& SelectedSlot);

	//**************************************************************
	// User Interaction
	//**************************************************************

	void IncrementToolbarSelection();
	void DecrementToolbarSelection();

	void SetToolbarSelectionIndex(const int8& SelectionIndex);

	void RemoveHeldItem();

	//**************************************************************
	// Getters
	//**************************************************************

	int8 GetToolbarSelectionIndex();
	
protected:

	virtual void OnInventoryChange() override;

private:

	UPROPERTY(Replicated, ReplicatedUsing = OnInventoryChange)
	int8 ToolbarSelectionIndex;

	UPROPERTY()
	class AWildOmissionCharacter* OwnerCharacter;

	//**************************************************************
	// Slot Functions
	//**************************************************************

	void RefreshToolbarSelectionState();
	
	bool IsToolbarSlotSelectionValid() const;

	//**************************************************************
	// RPC
	//**************************************************************

	UFUNCTION(Server, Reliable)
	void Server_SetToolbarSelectionIndex(int8 SelectionIndex);

};
