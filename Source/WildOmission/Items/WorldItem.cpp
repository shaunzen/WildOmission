// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/Core/Structs/ItemStat.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set this actor to replicate
	bReplicates = true;

	// Setup default values
	Item.Name = FName(TEXT("Item"));
	Item.Quantity = 1;

	// Create static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetIsReplicated(true);

	ConstructorHelpers::FObjectFinder<USoundBase> ClumpSoundObject(TEXT("/Game/WildOmission/Items/WorldItems/Audio/ItemClump_Cue"));
	ConstructorHelpers::FObjectFinder<USoundBase> PickupSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Pickup/Pickup_Cue"));
	
	if (!ClumpSoundObject.Succeeded() || !PickupSoundObject.Succeeded())
	{
		return;
	}

	ClumpSound = ClumpSoundObject.Object;
	PickupSound = PickupSoundObject.Object;
}

// Called when the game starts or when spawned
void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		return;
	}

	MeshComponent->SetMassOverrideInKg(FName(), 20.0f);
	MeshComponent->OnComponentHit.AddDynamic(this, &AWorldItem::OnComponentHit);
}

void AWorldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWorldItem, Item);
}

void AWorldItem::Interact(AActor* Interactor)
{
	UInventoryComponent* InteractorInventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InteractorInventoryComponent == nullptr)
	{
		return;
	}

	// Add to their inventory
	InteractorInventoryComponent->AddItem(Item);

	// Play Pickup sound
	Client_PlayPickupSound();

	// Destroy this Item
	Destroy();
}

FString AWorldItem::PromptText()
{
	FString ItemDisplayName;

	FItemData* ItemData = UWildOmissionStatics::GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *Item.Name.ToString());
	}

	ItemDisplayName = ItemData->DisplayName;

	return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemDisplayName);
}

void AWorldItem::SetItem(const FInventoryItem& InItem)
{
	FItemData* NewItemData = UWildOmissionStatics::GetItemData(InItem.Name);
	if (NewItemData == nullptr)
	{
		return;
	}

	MeshComponent->SetStaticMesh(NewItemData->Mesh);
	Item = InItem;
}

void AWorldItem::AddImpulse(FVector Impulse)
{
	MeshComponent->AddImpulse(Impulse);
}

FInventoryItem AWorldItem::GetItem() const
{
	return Item;
}

void AWorldItem::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AWorldItem* OtherWorldItem = Cast<AWorldItem>(OtherActor);
	if (OtherWorldItem == nullptr)
	{
		return;
	}

	FInventoryItem OurItem = this->GetItem();
	FInventoryItem OtherItem = OtherWorldItem->GetItem();
	FItemData* ItemData = UWildOmissionStatics::GetItemData(OurItem.Name);
	if (OtherItem.Name != OurItem.Name || ItemData == nullptr || ItemData->StackSize == 1)
	{
		return;
	}

	OurItem.Quantity = OtherItem.Quantity + OurItem.Quantity;
	int32 RemainingQuantity = 0;

	if (OurItem.Quantity > ItemData->StackSize)
	{
		RemainingQuantity = OurItem.Quantity - ItemData->StackSize;
		OurItem.Quantity = ItemData->StackSize;
		
		OtherItem.Quantity = RemainingQuantity;
		OtherWorldItem->SetItem(OtherItem);

		this->SetItem(OurItem);
	}
	else
	{
		OtherWorldItem->Destroy();

		this->SetItem(OurItem);
	}

	Client_PlayClumpSound();
}

void AWorldItem::Client_PlayClumpSound_Implementation()
{
	if (ClumpSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClumpSound, GetActorLocation());
}

void AWorldItem::Client_PlayPickupSound_Implementation()
{
	if (PickupSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
}