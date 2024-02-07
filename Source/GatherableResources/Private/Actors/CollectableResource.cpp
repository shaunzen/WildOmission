// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/CollectableResource.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "ChunkManager.h"
#include "Components/ChunkInvokerComponent.h"
#include "NavModifierComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectableResource::ACollectableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = false;
	NetUpdateFrequency = 5.0f;
	NetDormancy = DORM_DormantAll;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	
	Identifier = NAME_None;

	NavigationModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavigationModifier"));

	static ConstructorHelpers::FObjectFinder<USoundBase> CollectSoundObject(TEXT("/Game/WildOmissionCore/Characters/Human/Audio/Pickup/Pickup_Cue"));
	if (CollectSoundObject.Succeeded())
	{
		CollectSound = CollectSoundObject.Object;
	}

	Tags.Add(TEXT("Collectable"));
}

bool ACollectableResource::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	UChunkInvokerComponent* ChunkInvoker = ViewTarget->FindComponentByClass<UChunkInvokerComponent>();
	if (ChunkInvoker == nullptr)
	{
		return false;
	}
	
	const FVector CorrectedSrcLocation(SrcLocation.X, SrcLocation.Y, 0.0f);
	const FVector CorrectedThisLocation(this->GetActorLocation().X, this->GetActorLocation().Y, 0.0f);
	const float Distance = FVector::Distance(CorrectedSrcLocation, CorrectedThisLocation);

	return Distance < ChunkInvoker->GetRenderDistanceCentimeters();
}

void ACollectableResource::Interact(AActor* Interactor)
{
	UInventoryComponent* InteractorInventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InteractorInventoryComponent == nullptr)
	{
		return;
	}

	// Add resource to collectors inventory
	for (const FInventoryItem& YieldItem : Yield)
	{
		InteractorInventoryComponent->AddItem(YieldItem, true);
	}

	// Play Collect sound
	Client_PlayCollectSound();

	// Destroy the collectable
	Destroy();
}

FString ACollectableResource::PromptText()
{
	FString YieldedItemDisplayName;
	
	if (Yield.IsEmpty())
	{
		return TEXT("ERROR");
	}

	FItemData* ItemData = UInventoryComponent::GetItemData(Yield[0].Name);
	if (ItemData == nullptr)
	{
		return FString::Printf(TEXT("harvest %s"), *Yield[0].Name.ToString());
	}

	YieldedItemDisplayName = ItemData->DisplayName;

	return FString::Printf(TEXT("harvest %s"), *YieldedItemDisplayName);
}

FName ACollectableResource::GetIdentifier() const
{
	return Identifier;
}

void ACollectableResource::BeginPlay()
{
	Super::BeginPlay();

	// Get Chunk Manager
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	// Get Surface Type at location
	const uint8 SurfaceType = ChunkManager->GetSurfaceTypeAtLocation(this->GetActorLocation());
	
	// Set property acordingly
	MeshComponent->SetDefaultCustomPrimitiveDataFloat(0, static_cast<float>(SurfaceType == 6));
}

void ACollectableResource::Client_PlayCollectSound_Implementation()
{
	if (CollectSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSound, GetActorLocation());
}