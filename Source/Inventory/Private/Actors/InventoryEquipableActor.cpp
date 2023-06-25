// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/InventoryEquipableActor.h"

// Sets default values
AInventoryEquipableActor::AInventoryEquipableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventoryEquipableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryEquipableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

