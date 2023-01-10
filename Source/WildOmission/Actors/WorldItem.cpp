// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	ItemName = FName(TEXT("Item"));
	ItemQuantity = 1;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

// Called every frame
void AWorldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldItem::SetItemName(FName InName)
{
	ItemName = InName;
}

void AWorldItem::SetItemQuantity(int32 InQuantity)
{
	ItemQuantity = InQuantity;
}

void AWorldItem::SetItemMesh(UStaticMesh* InMesh)
{
	ItemMesh->SetStaticMesh(InMesh);
}

FName AWorldItem::GetItemName()
{
	return ItemName;
}

int32 AWorldItem::GetItemQuantity()
{
	return ItemQuantity;
}

UStaticMeshComponent* AWorldItem::GetItemMesh()
{
	return ItemMesh;
}