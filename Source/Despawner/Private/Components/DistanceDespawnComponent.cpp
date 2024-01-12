// Copyright Telephone Studios. All Rights Reserved.


#include "Components/DistanceDespawnComponent.h"
#include "ChunkManager.h"

// Sets default values for this component's properties
UDistanceDespawnComponent::UDistanceDespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// In the future this should be tied to render distance
	DespawnDistance = AChunkManager::GetRenderDistanceCentimeters();
}

// Called when the game starts
void UDistanceDespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	FTimerHandle CheckDespawnConditionTimerHandle;
	FTimerDelegate CheckDespawnConditionTimerDelegate;
	CheckDespawnConditionTimerDelegate.BindUObject(this, &UDistanceDespawnComponent::CheckDespawnConditions);
	GetWorld()->GetTimerManager().SetTimer(CheckDespawnConditionTimerHandle, CheckDespawnConditionTimerDelegate, 30.0f, true);
}

float UDistanceDespawnComponent::GetDespawnDistance() const
{
	return DespawnDistance;
}

void UDistanceDespawnComponent::SetDespawnDistance(const float& NewDespawnDistance)
{
	DespawnDistance = NewDespawnDistance;
}

void UDistanceDespawnComponent::CheckDespawnConditions()
{
	float DistanceFromClosestPlayer = -1.0f;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			continue;
		}

		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn == nullptr)
		{
			continue;
		}

		const FVector CorrectedPawnLocation(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y, 0.0f);
		const FVector CorrectedComponentLocation(this->GetComponentLocation().X, this->GetComponentLocation().Y, 0.0f);

		const float DistanceFromThisPlayer = FVector::Distance(CorrectedPawnLocation, CorrectedComponentLocation);
		if (DistanceFromThisPlayer < DistanceFromClosestPlayer || DistanceFromClosestPlayer == -1.0f)
		{
			DistanceFromClosestPlayer = DistanceFromThisPlayer;
		}
	}

	if (DistanceFromClosestPlayer > DespawnDistance && OnDespawnConditionMet.IsBound())
	{
		OnDespawnConditionMet.Broadcast();
	}
}