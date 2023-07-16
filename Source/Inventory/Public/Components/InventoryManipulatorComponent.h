// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/InventoryItem.h"
#include "InventoryManipulatorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryManipulatorOnSelectionChangedSignature, const FInventoryItem&, SelectedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenContainerChangedSignature, UInventoryComponent*, OpenContainer);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UInventoryManipulatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManipulatorComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UInventoryComponent* GetOpenContainer() const;
	UInventoryComponent* GetOwnersInventory() const;
	UFUNCTION(Server, Reliable)
	void Server_SetOpenContainer(UInventoryComponent* InOpenContainer);
	bool SelectedItemHasUniqueID(const uint32& UniqueID) const;

	FInventoryManipulatorOnSelectionChangedSignature OnSelectionChanged;
	FOnOpenContainerChangedSignature OnOpenContainerChanged;

	TArray<uint8> GetSelectedItemAsByteData();
	void LoadSelectedItemFromByteDataAndDropInWorld(const TArray<uint8>& ByteData);

	FInventoryItem GetSelectedItem();
	FInventoryItem* GetSelectedItemAddress();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, SaveGame)
	FInventoryItem SelectedItem;

	UPROPERTY(EditAnywhere)
	bool Dragging;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_OpenContainer)
	UInventoryComponent* OpenContainer;

	UFUNCTION()
	void OnRep_OpenContainer();

	UPROPERTY()
	UInventoryComponent* OwnersInventory;

	UFUNCTION(Server, Reliable)
	void Server_DropSelectedItemInWorld(bool Single);

	UFUNCTION()
	void BroadcastSelectionChanged();

};