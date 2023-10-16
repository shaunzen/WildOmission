// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Bed.h"
#include "Interfaces/BedController.h"
#include "Net/UnrealNetwork.h"

ABed::ABed()
{
	SpawnPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPointComponent"));
	SpawnPointComponent->SetupAttachment(MeshComponent);

	UniqueID = -1;
}

void ABed::OnSpawn()
{
	Super::OnSpawn();

	UniqueID = FMath::RandRange(0, 999999);
}

void ABed::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABed, UniqueID);
}

void ABed::Interact(AActor* Interactor)
{
	// TODO if this isn't the players current spawn point, then set it as the current spawn point
	APawn* InteractingPawn = Cast<APawn>(Interactor);
	if (InteractingPawn == nullptr)
	{
		return;
	}

	IBedController* BedController = Cast<IBedController>(InteractingPawn->GetController());
	if (BedController == nullptr)
	{
		return;
	}

	BedController->SetBedUniqueID(this->UniqueID);
}

FString ABed::PromptText()
{
	IBedController* BedController = Cast<IBedController>(GetWorld()->GetFirstPlayerController());
	if (BedController == nullptr)
	{
		return TEXT("ERROR");
	}

	const bool IsCurrentSpawnPoint = BedController->GetBedUniqueID() == this->UniqueID;

	return IsCurrentSpawnPoint ? TEXT("NOPRESSPROMPT_Current Spawn Point") : TEXT("to set spawn");
}

USceneComponent* ABed::GetSpawnPointComponent() const
{
	return SpawnPointComponent;
}

int32 ABed::GetUniqueID() const
{
	return UniqueID;
}
