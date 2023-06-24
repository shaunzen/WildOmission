// Copyright Telephone Studios. All Rights Reserved.


#include "CollectableResource.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Core/WildOmissionStatics.h"

// Sets default values
ACollectableResource::ACollectableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 5.0f;
	NetDormancy = DORM_DormantAll;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	ConstructorHelpers::FObjectFinder<USoundBase> CollectSoundObject(TEXT("/Game/WildOmission/Characters/Human/Audio/Pickup/Pickup_Cue"));
	if (CollectSoundObject.Succeeded())
	{
		CollectSound = CollectSoundObject.Object;
	}

	Tags.Add(FName("Collectable"));
}

void ACollectableResource::Interact(AActor* Interactor)
{
	UInventoryComponent* InteractorInventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InteractorInventoryComponent == nullptr)
	{
		return;
	}

	// Add resource to collectors inventory
	InteractorInventoryComponent->AddItem(Yield);

	// Play Collect sound
	Client_PlayCollectSound();

	// Destroy the collectable
	Destroy();
}

FString ACollectableResource::PromptText()
{
	FString YieldedItemDisplayName;

	FItemData* ItemData = UInventoryComponent::GetItemData(Yield.Name);
	if (ItemData == nullptr)
	{
		return FString::Printf(TEXT("Press 'E' to harvest %s"), *Yield.Name.ToString());
	}

	YieldedItemDisplayName = ItemData->DisplayName;

	return FString::Printf(TEXT("Press 'E' to harvest %s"), *YieldedItemDisplayName);
}

void ACollectableResource::Client_PlayCollectSound_Implementation()
{
	if (CollectSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSound, GetActorLocation());
}