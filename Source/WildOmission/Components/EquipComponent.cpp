// Copyright Telephone Studios. All Rights Reserved.


#include "EquipComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Items/EquipableItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

void UEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipComponent, EquipedItem);
}

// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipComponent::EquipItem(TSubclassOf<AEquipableItem> Item)
{
	AWildOmissionCharacter* OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	EquipedItem = GetWorld()->SpawnActor<AEquipableItem>(Item, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());

	EquipedItem->Equip(OwnerCharacter);
}

void UEquipComponent::Disarm()
{
	if (!IsItemEquiped())
	{
		return;
	}

	EquipedItem->OnUnequip();
	EquipedItem->Destroy();

	EquipedItem = nullptr;
}

AEquipableItem* UEquipComponent::GetEquipedItem()
{
	return EquipedItem;
}

bool UEquipComponent::IsItemEquiped() const
{
	return EquipedItem != nullptr;
}

// RPC
void UEquipComponent::Server_Primary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Primary();
}

void UEquipComponent::Server_Secondary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Secondary();
}