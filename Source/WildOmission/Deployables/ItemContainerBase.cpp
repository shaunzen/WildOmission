// Copyright Telephone Studios. All Rights Reserved.


#include "ItemContainerBase.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Net/UnrealNetwork.h"

AItemContainerBase::AItemContainerBase()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));
	WidgetClass = nullptr;
	bOccupied = false;

	ContainerName = TEXT("Container");
}

void AItemContainerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemContainerBase, bOccupied);
}

void AItemContainerBase::Interact(AActor* Interactor)
{
	if (bOccupied == true)
	{
		return;
	}

	AWildOmissionCharacter* InteractingCharacter = Cast<AWildOmissionCharacter>(Interactor);
	if (InteractingCharacter == nullptr)
	{
		return;
	}

	SetOwner(Interactor);

	InteractingCharacter->Client_OpenContainer(this);
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
	bOccupied = false;
}

UClass* AItemContainerBase::GetWidgetClass() const
{
	return WidgetClass.Get();
}

UInventoryComponent* AItemContainerBase::GetInventoryComponent() const
{
	return InventoryComponent;
}