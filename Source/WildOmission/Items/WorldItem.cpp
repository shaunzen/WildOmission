// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"

// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to replicate
	bReplicates = true;

	// Setup default values
	ItemName = FName(TEXT("Item"));
	ItemQuantity = 1;

	// Create static mesh component
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

void AWorldItem::Interact(AActor* Interactor)
{
	AWildOmissionCharacter* CharacterInteractor = Cast<AWildOmissionCharacter>(Interactor);
	if (CharacterInteractor == nullptr)
	{
		return;
	}
	AWildOmissionPlayerController* PlayerControllerInteractor = Cast<AWildOmissionPlayerController>(CharacterInteractor->GetController());
	if (PlayerControllerInteractor == nullptr)
	{
		return;
	}

	// Add to their inventory
	CharacterInteractor->GetInventoryComponent()->AddItem(ItemName, ItemQuantity);
	
	// Destroy this Item
	Destroy();
	PlayerControllerInteractor->Server_DestroyActor(this);
}

FString AWorldItem::PromptText()
{
	return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemName.ToString());
}

// Sets the item id name for this world item
void AWorldItem::Client_SetItemName_Implementation(FName InName)
{
	ItemName = InName;
}

// Sets the item quantity for this world item
void AWorldItem::Client_SetItemQuantity_Implementation(int32 InQuantity)
{
	ItemQuantity = InQuantity;
}

// Sets the item mesh for this world item
void AWorldItem::Client_SetItemMesh_Implementation(UStaticMesh* InMesh)
{
	ItemMesh->SetStaticMesh(InMesh);
}

// Sets all properties for this world item in one go
void AWorldItem::Client_SetItemProperties_Implementation(FName InName, int32 InQuantity, UStaticMesh* InMesh, FVector InLocation)
{
	ItemName = InName;
	ItemQuantity = InQuantity;
	ItemMesh->SetStaticMesh(InMesh);
	SetActorLocation(InLocation);
}

// Gets the item name
FName AWorldItem::GetItemName()
{
	return ItemName;
}

// Gets the item quantity
int32 AWorldItem::GetItemQuantity()
{
	return ItemQuantity;
}

// Gets the items static mesh component
UStaticMeshComponent* AWorldItem::GetItemMesh()
{
	return ItemMesh;
}
