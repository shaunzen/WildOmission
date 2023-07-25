// Copyright Telephone Studios. All Rights Reserved.


#include "Components/DistanceDespawnComponent.h"

// Sets default values for this component's properties
UDistanceDespawnComponent::UDistanceDespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	DespawnDistance = 100000.0f;
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
		if (PlayerController == nullptr || PlayerController->GetPawn() == nullptr)
		{
			continue;
		}

		const float DistanceFromThisPlayer = FVector::Distance(PlayerController->GetPawn()->GetActorLocation(), GetComponentLocation());
		if (DistanceFromThisPlayer < DistanceFromClosestPlayer || DistanceFromClosestPlayer == -1.0f)
		{
			DistanceFromClosestPlayer = DistanceFromThisPlayer;
		}
	}
}