// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/ItemContainerBase.h"
#include "Components/InventoryComponent.h"
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

	IContainerOpener* InteractingContainerOpener = Cast<IContainerOpener>(Interactor);
	if (InteractingContainerOpener == nullptr)
	{
		return;
	}

	SetOwner(Interactor);

	InteractingContainerOpener->InvokeOpenContainer(this);
	bOccupied = true;
}

FString AItemContainerBase::PromptText()
{
	if (bOccupied == false)
	{
		return FString::Printf(TEXT("Press 'E' to open %s"), *ContainerName);
	}

	return FString("Occupied");
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
	bOccupied = false;
}
