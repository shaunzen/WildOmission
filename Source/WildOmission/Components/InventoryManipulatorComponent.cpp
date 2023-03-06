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
	
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

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

void UInventoryManipulatorComponent::StartDragging(const FInventoryItem& ItemToDrag)
{
	SelectedItem = ItemToDrag;
	Dragging = true;
}

void UInventoryManipulatorComponent::StopDragging(bool DropInWorld)
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
		SpawnWorldItem(SelectedItemInformation.Name, SelectedItemInformation.Quantity, SelectedItemInformation.Stats);
	}
}

void UInventoryManipulatorComponent::SpawnWorldItem(const FName& ItemName, const int32& Quantity, const TArray<FItemStat>& Stats)
{
	// Get player's inventory
	UPlayerInventoryComponent* PlayerInventoryComponent = GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
	if (PlayerInventoryComponent == nullptr)
	{
		return;
	}

	// Get the data for this item
	FItemData* ItemData = PlayerInventoryComponent->GetItemData(ItemName);

	// Spawn a world item actor
	AWorldItem* WorldItem = GetWorld()->SpawnActor<AWorldItem>();
	if (ItemData == nullptr || WorldItem == nullptr)
	{
		return;
	}

	WorldItem->SetOwner(GetOwner());

	FVector SpawnLocation;
	FVector PhysicsImpulse;

	SpawnLocation = GetOwner()->GetActorLocation();
	PhysicsImpulse = GetOwner()->GetActorForwardVector() * 5000.0f;

	// Update world items properties
	WorldItem->SetActorLocation(SpawnLocation);
	WorldItem->SetName(ItemName);
	WorldItem->SetQuantity(Quantity);
	WorldItem->SetStats(Stats);
	WorldItem->SetMesh(ItemData->Mesh);
	WorldItem->AddImpulse(PhysicsImpulse);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString("hahah spoon the actor lmao"));
}

//**************************************************************
// User Interaction
//**************************************************************

void UInventoryManipulatorComponent::Server_DropSelectedItemInWorld_Implementation(bool Single)
{
	if (!Dragging)
	{
		return;
	}
	if (Single == true)
	{
		SpawnWorldItem(SelectedItem.Name, 1, SelectedItem.Stats);
		SelectedItem.Quantity -= 1;
	}
	else
	{
		SpawnWorldItem(SelectedItem.Name, SelectedItem.Quantity, SelectedItem.Stats);
		SelectedItem.Clear();
	}

	if (SelectedItem.Quantity == 0)
	{
		SelectedItem.Clear();
		Dragging = false;
	}

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