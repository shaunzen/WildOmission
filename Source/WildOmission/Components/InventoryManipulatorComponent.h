// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "InventoryManipulatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryManipulatorComponent : public UActorComponent
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
	void SpawnWorldItem(const FInventoryItem& ItemToSpawn);

	//**************************************************************
	// User Interaction
	//**************************************************************

	UFUNCTION(Server, Reliable)
	void Server_DropSelectedItemInWorld(bool Single);

	//**************************************************************
	// Getters
	//**************************************************************
	
	bool IsDragging() const;

	bool SelectedItemHasUniqueID(const uint32& UniqueID) const;

	FInventoryItem GetSelectedItem();
	FInventoryItem* GetSelectedItemAddress();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = RefreshUI)
	FInventoryItem SelectedItem;

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = RefreshUI)
	bool Dragging;

	UFUNCTION()
	void RefreshUI();

};
