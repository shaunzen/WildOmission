// Copyright Telephone Studios. All Rights Reserved.


#include "EquipableItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipableItem::Equip(AWildOmissionCharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;
	SetOwner(InOwnerCharacter);
}

void AEquipableItem::Primary()
{

}

void AEquipableItem::Secondary()
{

}