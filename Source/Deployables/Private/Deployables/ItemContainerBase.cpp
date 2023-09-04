// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/ItemContainerBase.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Interfaces/ContainerOpener.h"
#include "Net/UnrealNetwork.h"

AItemContainerBase::AItemContainerBase()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));

	bOccupied = false;

	ContainerName = TEXT("Container");
}

void AItemContainerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemContainerBase, bOccupied);
}

void AItemContainerBase::UnOccupy()
{
	Server_UnOccupy();
}

void AItemContainerBase::Interact(AActor* Interactor)
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

FString AItemContainerBase::PromptText()
{
	if (bOccupied == false)
	{
		return FString::Printf(TEXT("open %s"), *ContainerName);
	}

	return TEXT("NOPRESSPROMPT_Occupied");
}

void AItemContainerBase::Server_UnOccupy_Implementation()
{
	OnContainerClosed();
	SetNetDormancy(ENetDormancy::DORM_DormantAll);
}

FString AItemContainerBase::GetContainerName() const
{
	return ContainerName;
}

UInventoryComponent* AItemContainerBase::GetInventoryComponent() const
{
	return InventoryComponent;
}

void AItemContainerBase::OnContainerClosed()
{
	UInventoryManipulatorComponent* OwnerInventoryManipulator = GetOwner()->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulator == nullptr)
	{
		return;
	}

	OwnerInventoryManipulator->SetOpenContainer(nullptr);
	bOccupied = false;
}
