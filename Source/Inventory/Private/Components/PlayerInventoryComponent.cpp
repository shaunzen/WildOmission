// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "WildOmissionGameUserSettings.h"
#include "WorldItem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"


// Get Settings, store if it's using the alpha scroll

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	SetIsReplicatedByDefault(true);

	SlotCount = 30;
	ToolbarSelectionIndex = -1;

	ItemBreakSound = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundBase> ItemBreakSoundBlueprint(TEXT("/Game/EquipableItems/Audio/A_Tool_Break_02"));
	if (ItemBreakSoundBlueprint.Succeeded())
	{
		ItemBreakSound = ItemBreakSoundBlueprint.Object;
	}
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UInventoryManipulatorComponent* OwnerManipulator = GetOwner()->FindComponentByClass<UInventoryManipulatorComponent>();

	

	ToolbarSelectionIndex = -1;

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	OnUpdate.AddDynamic(this, &UPlayerInventoryComponent::RefreshToolbarSelectionState);

	if (!WasLoadedFromSave())
	{
		FInventoryItem RockItem;
		RockItem.Name = TEXT("rock");
		RockItem.Quantity = 1;

		AddItem(RockItem);

		FInventoryItem TorchItem;
		TorchItem.Name = TEXT("torch");
		TorchItem.Quantity = 1;

		AddItem(TorchItem);

		FInventoryItem MapItem;
		MapItem.Name = TEXT("map");
		MapItem.Quantity = 1;

		AddItem(MapItem);
	}
}

void UPlayerInventoryComponent::OnRep_ServerState()
{
	Super::OnRep_ServerState();

	RefreshToolbarSelectionState();
}

//**************************************************************
// User Interaction
//**************************************************************

void UPlayerInventoryComponent::IncrementToolbarSelection()
{
							// Invert input if Alpha Scroll is enabled
	SetToolbarSelectionIndex(UseAlphaToolbarScroll() ? ToolbarSelectionIndex - 1 : ToolbarSelectionIndex + 1);
}

void UPlayerInventoryComponent::DecrementToolbarSelection()
{
							// Invert input if Alpha Scroll is enabled
	SetToolbarSelectionIndex(UseAlphaToolbarScroll() ? ToolbarSelectionIndex + 1 : ToolbarSelectionIndex - 1);
}

void UPlayerInventoryComponent::SetToolbarSelectionIndex(int8 SelectionIndex)
{
	if (SelectionIndex == -2 || SelectionIndex > 5)
	{
		SelectionIndex = 0;
	}
	else if (SelectionIndex < 0)
	{
		SelectionIndex = 5;
	}

	ToolbarSelectionIndex = SelectionIndex;

	Server_SetToolbarSelectionIndex(ToolbarSelectionIndex);

	RefreshToolbarSelectionState();
	BroadcastInventoryUpdate();
}

void UPlayerInventoryComponent::RemoveHeldItem()
{
	FInventorySlot& SelectedSlot = ServerState.Slots[ToolbarSelectionIndex];

	ServerState.Contents.RemoveItem(SelectedSlot.Item.Name, 1);

	--SelectedSlot.Item.Quantity;

	if (SelectedSlot.IsEmpty())
	{
		SelectedSlot.ClearItem();
	}

	RefreshToolbarSelectionState();
	OnRep_ServerState();
}

//**************************************************************
// Getters
//**************************************************************

int8 UPlayerInventoryComponent::GetToolbarSelectionIndex()
{
	return ToolbarSelectionIndex;
}

//**************************************************************
// Slot Functions
//**************************************************************

void UPlayerInventoryComponent::RefreshToolbarSelectionState()
{
	if (!OnToolbarSlotSelectionChange.IsBound() || !IsToolbarSlotSelectionValid())
	{
		return;
	}

	FInventorySlot& SelectedSlot = ServerState.Slots[ToolbarSelectionIndex];
	OnToolbarSlotSelectionChange.Broadcast(ToolbarSelectionIndex, SelectedSlot);
}

bool UPlayerInventoryComponent::IsToolbarSlotSelectionValid() const
{
	return ToolbarSelectionIndex > -1 && ToolbarSelectionIndex < 6;
}

// Unfortionatly this has to be called every time the player scrolls the toolbar,
// I tried only running this at begin play but it seems the settings context doesn't exist yet
// or is unreachable
bool UPlayerInventoryComponent::UseAlphaToolbarScroll() const
{
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return false;
	}

	return UserSettings->GetAlphaToolbarScroll();
}

//**************************************************************
// RPC
//**************************************************************

bool UPlayerInventoryComponent::Server_SetToolbarSelectionIndex_Validate(int8 SelectionIndex)
{
	return SelectionIndex >= 0 && SelectionIndex <= 5;
}

void UPlayerInventoryComponent::Server_SetToolbarSelectionIndex_Implementation(int8 SelectionIndex)
{
	ToolbarSelectionIndex = SelectionIndex;

	RefreshToolbarSelectionState();
}

void UPlayerInventoryComponent::Multi_PlayItemBreakSound_Implementation()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ItemBreakSound, OwnerActor->GetActorLocation());
}