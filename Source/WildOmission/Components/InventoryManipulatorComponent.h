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

	void SpawnWorldItem(const FName& ItemName, const int32& Quantity);

	//**************************************************************
	// User Interaction
	//**************************************************************

	void DropSelectedItemInWorld(bool Single = false);

	void StartDragging(const FInventoryItem& ItemToDrag);
	void StopDragging(bool DropInWorld = false);

	//**************************************************************
	// Getters
	//**************************************************************
	
	bool IsDragging() const;

	FInventoryItem GetSelectedItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY(Replicated, ReplicatedUsing = RefreshUI)
	FInventoryItem SelectedItem;

	UPROPERTY(Replicated, ReplicatedUsing = RefreshUI)
	bool Dragging;

	UFUNCTION()
	void RefreshUI();

	//**************************************************************
	// RPC
	//**************************************************************

	UFUNCTION(Server, Reliable)
	void Server_DropSelectedItemInWorld(bool Single);

	UFUNCTION(Server, Reliable)
	void Server_SpawnWorldItem(const FName& ItemName, const int32& Quantity);

	UFUNCTION(Server, Reliable)
	void Server_StartDragging(const FInventoryItem& ItemToDrag);

	UFUNCTION(Server, Reliable)
	void Server_StopDragging(bool DropInWorld);

};
