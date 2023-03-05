// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryManipulatorComponent.h"
#include "WildOmission/Items/WorldItem.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryManipulatorComponent::UInventoryManipulatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	Dragging = false;
}


// Called when the game starts
void UInventoryManipulatorComponent::BeginPlay()
{
	Super::BeginPlay();

	Dragging = false;
	SelectedItem.Clear();
	
	RefreshUI();
}

void UInventoryManipulatorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryManipulatorComponent, SelectedItem);
	DOREPLIFETIME(UInventoryManipulatorComponent, Dragging);
}

//**************************************************************
// General Management
//**************************************************************

void UInventoryManipulatorComponent::SpawnWorldItem(const FName& ItemName, const int32& Quantity)
{
	// RPC on the server to spawn a world item of out specification
	Server_SpawnWorldItem(ItemName, Quantity);
}

//**************************************************************
// User Interaction
//**************************************************************

void UInventoryManipulatorComponent::DropSelectedItemInWorld(bool Single)
{
	if (!Dragging)
	{
		return;
	}

	Server_DropSelectedItemInWorld(Single);

	RefreshUI();
}

void UInventoryManipulatorComponent::StartDragging(const FInventoryItem& ItemToDrag)
{
	Server_StartDragging(ItemToDrag);
}

void UInventoryManipulatorComponent::StopDragging(bool DropInWorld)
{
	Server_StopDragging(DropInWorld);
}

//**************************************************************
// Getters
//**************************************************************

bool UInventoryManipulatorComponent::IsDragging() const
{
	return Dragging;
}

bool UInventoryManipulatorComponent::SelectedItemHasUniqueID(const uint32& UniqueID) const
{
	if (Dragging == false || SelectedItem.UniqueID != UniqueID)
	{
		return false;
	}

	return true;
}

FInventoryItem UInventoryManipulatorComponent::GetSelectedItem()
{
	return SelectedItem;
}

void UInventoryManipulatorComponent::RefreshUI()
{
	AWildOmissionCharacter* OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	// get the player hud
	UPlayerHUDWidget* OwnerHUDWidget = OwnerCharacter->GetHUDWidget();
	if (OwnerHUDWidget == nullptr)
	{
		return;
	}

	// call refresh
	OwnerHUDWidget->RefreshInventoryStates();
}

//**************************************************************
// RPC
//**************************************************************

void UInventoryManipulatorComponent::Server_DropSelectedItemInWorld_Implementation(bool Single)
{
	if (!Dragging)
	{
		return;
	}
	if (Single == true)
	{
		Server_SpawnWorldItem(SelectedItem.Name, 1);
		SelectedItem.Quantity -= 1;
	}
	else
	{
		Server_SpawnWorldItem(SelectedItem.Name, SelectedItem.Quantity);
		SelectedItem.Clear();
	}

	if (SelectedItem.Quantity == 0)
	{
		SelectedItem.Clear();
		Dragging = false;
	}

}

void UInventoryManipulatorComponent::Server_SpawnWorldItem_Implementation(FName ItemName, int32 Quantity)
{
	// Get player's inventory
	UPlayerInventoryComponent* PlayerInventoryComponent = GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
	if (PlayerInventoryComponent == nullptr)
	{
		return;
	}

	// Get the data for this item
	FItem* ItemData = PlayerInventoryComponent->GetItemData(ItemName);
	
	// Spawn a world item actor
	AWorldItem* WorldItem = GetWorld()->SpawnActor<AWorldItem>();
	if (ItemData == nullptr || WorldItem == nullptr)
	{
		return;
	}

	FVector SpawnLocation;
	FVector PhysicsImpulse;

	SpawnLocation = GetOwner()->GetActorLocation();
	PhysicsImpulse = GetOwner()->GetActorForwardVector() * 5000.0f;

	// Update world items properties
	WorldItem->Client_SetItemProperties(ItemName, Quantity, ItemData->Mesh, SpawnLocation);

	WorldItem->AddImpulse(PhysicsImpulse);
}

void UInventoryManipulatorComponent::Server_StartDragging_Implementation(FInventoryItem ItemToDrag)
{
	SelectedItem = ItemToDrag;
	Dragging = true;
}

void UInventoryManipulatorComponent::Server_StopDragging_Implementation(bool DropInWorld)
{
	if (Dragging == false)
	{
		return;
	}

	FInventoryItem SelectedItemInformation = SelectedItem;

	SelectedItem.Clear();
	Dragging = false;

	if (DropInWorld)
	{
		SpawnWorldItem(SelectedItemInformation.Name, SelectedItemInformation.Quantity);
	}
}
