// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

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

}

// Called every frame
void AWorldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldItem::SetItemName_Implementation(FName InName)
{
	ItemName = InName;
}

void AWorldItem::SetItemQuantity_Implementation(int32 InQuantity)
{
	ItemQuantity = InQuantity;
}

void AWorldItem::SetItemMesh_Implementation(UStaticMesh* InMesh)
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
/*
void AWorldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWorldItem, ItemName);
	DOREPLIFETIME(AWorldItem, ItemQuantity);
	DOREPLIFETIME(AWorldItem, ItemMesh);
}
*/