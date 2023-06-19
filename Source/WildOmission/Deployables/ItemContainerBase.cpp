// Copyright Telephone Studios. All Rights Reserved.


#include "ItemContainerBase.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

AItemContainerBase::AItemContainerBase()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));

	ConstructorHelpers::FClassFinder<UUserWidget> DefaultWidgetBlueprint(TEXT("/Game/WildOmission/UI/Inventory/WBP_ContainerBase"));
	if (DefaultWidgetBlueprint.Succeeded())
	{
		WidgetClass = DefaultWidgetBlueprint.Class;
	}

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

FString AItemContainerBase::GetContainerName() const
{
	return ContainerName;
}

UClass* AItemContainerBase::GetWidgetClass() const
{
	return WidgetClass.Get();
}

UInventoryComponent* AItemContainerBase::GetInventoryComponent() const
{
	return InventoryComponent;
}