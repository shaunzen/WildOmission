// Copyright Telephone Studios. All Rights Reserved.


#include "StorageCrate.h"
#include "WildOmission/Components/InventoryComponent.h"

// Sets default values
AStorageCrate::AStorageCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	RootComponent = Mesh;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));

}

// Called when the game starts or when spawned
void AStorageCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStorageCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStorageCrate::Interact(AActor* Interactor)
{
	// RPC to client of user trying to open
}

FString AStorageCrate::PromptText()
{
	return FString("Press 'E' to open Storage Crate");
}