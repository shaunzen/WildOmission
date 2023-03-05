// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set this actor to replicate
	bReplicates = true;

	// Setup default values
	ItemName = FName(TEXT("Item"));
	ItemQuantity = 1;

	// Create static mesh component
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	ConstructorHelpers::FObjectFinder<USoundBase> PickupSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Pickup/Pickup_Cue"));
	
	if (PickupSoundObject.Object == nullptr)
	{
		return;
	}

	PickupSound = PickupSoundObject.Object;
}

// Called when the game starts or when spawned
void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	
	ItemMesh->SetMassOverrideInKg(FName(), 20.0f);
	SetReplicateMovement(true);
}

void AWorldItem::Interact(AActor* Interactor)
{
	UInventoryComponent* InteractorInventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InteractorInventoryComponent == nullptr)
	{
		return;
	}

	// Add to their inventory
	InteractorInventoryComponent->AddItem(ItemName, ItemQuantity);

	// Play Pickup sound
	Client_PlayPickupSound();

	// Destroy this Item
	Destroy();
}

FString AWorldItem::PromptText()
{
	FName ItemDisplayName;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemName.ToString());
	}

	AWildOmissionCharacter* Character = Cast<AWildOmissionCharacter>(PlayerController->GetCharacter());
	if (Character == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemName.ToString());
	}

	UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
	if (PlayerInventoryComponent == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemName.ToString());
	}

	FItem* ItemData = PlayerInventoryComponent->GetItemData(ItemName);
	if (ItemData == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemName.ToString());
	}

	ItemDisplayName = ItemData->DisplayName;

	return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemDisplayName.ToString());
}

// Sets all properties for this world item in one go
void AWorldItem::Client_SetItemProperties_Implementation(FName InName, int32 InQuantity, const TArray<FItemStat>& InStats, UStaticMesh* InMesh, FVector InLocation)
{
	ItemName = InName;
	ItemQuantity = InQuantity;
	Stats = InStats;
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

TArray<FItemStat> AWorldItem::GetStats()
{
	return Stats;
}

// Gets the items static mesh component
UStaticMeshComponent* AWorldItem::GetItemMesh()
{
	return ItemMesh;
}

void AWorldItem::AddImpulse(FVector Impulse)
{
	ItemMesh->AddImpulse(Impulse);
}

void AWorldItem::Client_PlayPickupSound_Implementation()
{
	if (GetWorld() == nullptr || PickupSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
}