// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/InventoryItem.h"
#include "InventoryManipulatorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryManipulatorOnSelectionChangedSignature, const FInventoryItem&, SelectedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UInventoryManipulatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManipulatorComponent();
	
	//**************************************************************
	// General Management
	//**************************************************************

	void StartDragging(const FInventoryItem& ItemToDrag);
	void StopDragging();

	//**************************************************************
	// User Interaction
	//**************************************************************

	void DropSelectedItemInWorld(bool Single);

	//**************************************************************
	// Getters
	//**************************************************************
	
	bool IsDragging() const;

	bool SelectedItemHasUniqueID(const uint32& UniqueID) const;

	FInventoryManipulatorOnSelectionChangedSignature OnSelectionChanged;

	FInventoryItem GetSelectedItem();
	FInventoryItem* GetSelectedItemAddress();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere)
	FInventoryItem SelectedItem;

	UPROPERTY(EditAnywhere)
	bool Dragging;

	UFUNCTION(Server, Reliable)
	void Server_DropSelectedItemInWorld(bool Single);

	UFUNCTION()
	void BroadcastSelectionChanged();
};
