// Copyright Telephone Studios. All Rights Reserved.


#include "ContainerBase.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AContainerBase::AContainerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));

	WidgetClass = nullptr;

	bOccupied = false;
}

void AContainerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AContainerBase, bOccupied);
}

// Called when the game starts or when spawned
void AContainerBase::BeginPlay()
{
	Super::BeginPlay();
	
	bOccupied = false;
}

// Called every frame
void AContainerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AContainerBase::Interact(AActor* Interactor)
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

FString AContainerBase::PromptText()
{
	if (bOccupied == false)
	{
		return FString("Press 'E' to open Container");
	}

	return FString("Occupied");
}

void AContainerBase::Server_UnOccupy_Implementation()
{
	bOccupied = false;
}

UClass* AContainerBase::GetWidgetClass() const
{
	return WidgetClass.Get();
}

UInventoryComponent* AContainerBase::GetInventoryComponent() const
{
	return InventoryComponent;
}