// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/Core/Structs/ItemStat.h"
#include "Kismet/GameplayStatics.h"
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
	MeshComponent->SetIsReplicated(true);

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
	
	if (!HasAuthority())
	{
		return;
	}

	MeshComponent->SetMassOverrideInKg(FName(), 20.0f);
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

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *Item.Name.ToString());
	}

	AWildOmissionCharacter* Character = Cast<AWildOmissionCharacter>(PlayerController->GetCharacter());
	if (Character == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *Item.Name.ToString());
	}

	UInventoryComponent* PlayerInventoryComponent = Character->GetInventoryComponent();
	if (PlayerInventoryComponent == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *Item.Name.ToString());
	}

	FItemData* ItemData = PlayerInventoryComponent->GetItemData(Item.Name);
	if (ItemData == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to pickup %s"), *Item.Name.ToString());
	}

	ItemDisplayName = ItemData->DisplayName;

	return FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemDisplayName);
}

void AWorldItem::SetItem(const FInventoryItem& InItem)
{
	UInventoryComponent* FirstPlayerInventoryComponent = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UInventoryComponent>();
	if (FirstPlayerInventoryComponent == nullptr)
	{
		return;
	}
	
	FItemData* NewItemData = FirstPlayerInventoryComponent->GetItemData(InItem.Name);
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

void AWorldItem::Client_PlayPickupSound_Implementation()
{
	if (GetWorld() == nullptr || PickupSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
}