// (c) 2023 Telephone Studios, all rights reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemName = FName(TEXT("Item"));
	ItemQuantity = 1;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	ItemMesh->SetupAttachment(RootComponent);
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