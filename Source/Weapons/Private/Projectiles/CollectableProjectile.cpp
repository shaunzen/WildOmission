// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/CollectableProjectile.h"
#include "ChunkManager.h"
#include "Components/ChunkInvokerComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// TODO this needs to attach to the nearest chunk!

// Sets default values
ACollectableProjectile::ACollectableProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootSceneComponent);
	StaticMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Block);

	ItemID = NAME_None;

	static ConstructorHelpers::FObjectFinder<USoundBase> CollectSoundObject(TEXT("/Game/WildOmissionCore/Characters/Human/Audio/Pickup/Pickup_Cue"));
	if (CollectSoundObject.Succeeded())
	{
		CollectSound = CollectSoundObject.Object;
	}
}

bool ACollectableProjectile::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	UChunkInvokerComponent* ChunkInvoker = RealViewer->FindComponentByClass<UChunkInvokerComponent>();
	if (ChunkInvoker == nullptr)
	{
		return false;
	}

	const FVector CorrectedSrcLocation(SrcLocation.X, SrcLocation.Y, 0.0f);
	const FVector CorrectedThisLocation(this->GetActorLocation().X, this->GetActorLocation().Y, 0.0f);
	float Distance = FVector::Distance(CorrectedSrcLocation, CorrectedThisLocation);

	return Distance < ChunkInvoker->GetRenderDistanceCentimeters();
}

void ACollectableProjectile::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		return;
	}

	UInventoryComponent* InteractorInventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InteractorInventoryComponent == nullptr)
	{
		return;
	}

	FInventoryItem ProjectileItem;
	ProjectileItem.Name = ItemID;
	ProjectileItem.Quantity = 1;
	InteractorInventoryComponent->AddItem(ProjectileItem);

	Multi_PlayCollectSound();

	Destroy();
}

FString ACollectableProjectile::PromptText()
{
	FItemData* ProjectileItemData = UInventoryComponent::GetItemData(ItemID);
	if (ProjectileItemData == nullptr)
	{
		return FString::Printf(TEXT("collect %s"), *ItemID.ToString());
	}

	return FString::Printf(TEXT("collect %s"), *ProjectileItemData->DisplayName);
}

void ACollectableProjectile::Multi_PlayCollectSound_Implementation()
{
	if (GetWorld() == nullptr || CollectSound == nullptr)
	{
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSound, GetActorLocation());
}