// Copyright Telephone Studios. All Rights Reserved.


#include "Ragdolls/LootableRagdoll.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Net/UnrealNetwork.h"

ALootableRagdoll::ALootableRagdoll()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetSlotCount(30);
	InventoryComponent->SetDisplayName(TEXT("Inventory"));

	bOccupied = false;

	// TODO set to PlayerName's Inventory
	ContainerName = TEXT("Inventory");
}

void ALootableRagdoll::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALootableRagdoll, bOccupied);
}

void ALootableRagdoll::UnOccupy()
{
	Server_UnOccupy();
}

void ALootableRagdoll::Interact(AActor* Interactor)
{
	if (bOccupied == true)
	{
		return;
	}

	SetNetDormancy(ENetDormancy::DORM_Awake);

	UInventoryManipulatorComponent* InteractorInventoryManipulator = Interactor->FindComponentByClass<UInventoryManipulatorComponent>();
	if (InteractorInventoryManipulator == nullptr)
	{
		return;
	}

	SetOwner(Interactor);
	InteractorInventoryManipulator->SetOpenContainer(InventoryComponent);

	bOccupied = true;
}

FString ALootableRagdoll::PromptText()
{
	if (bOccupied == false)
	{
		return FString::Printf(TEXT("open %s"), *ContainerName);
	}

	return TEXT("NOPRESSPROMPT_Occupied");
}

FString ALootableRagdoll::GetContainerName() const
{
	return ContainerName;
}

UInventoryComponent* ALootableRagdoll::GetInventoryComponent() const
{
	return InventoryComponent;
}

// TODO hmm this could be done better i think
void ALootableRagdoll::OnContainerClosed()
{
	UInventoryManipulatorComponent* OwnerInventoryManipulator = GetOwner()->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulator == nullptr)
	{
		return;
	}

	OwnerInventoryManipulator->SetOpenContainer(nullptr);
	bOccupied = false;
}

void ALootableRagdoll::Server_UnOccupy_Implementation()
{
	OnContainerClosed();
	SetNetDormancy(ENetDormancy::DORM_DormantAll);
}
