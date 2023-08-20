// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/TemporaryItemContainer.h"
#include "Components/TimerDespawnComponent.h"

ATemporaryItemContainer::ATemporaryItemContainer()
{
	DespawnComponent = CreateDefaultSubobject<UTimerDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->SetDespawnTime(1200.0f);

	CanBeInteractedWith = true;
}

void ATemporaryItemContainer::Interact(AActor* Interactor)
{
	if (CanBeInteractedWith == false)
	{
		return;
	}

	Super::Interact(Interactor);
}

FString ATemporaryItemContainer::PromptText()
{
	if (CanBeInteractedWith == false)
	{
		return TEXT("");
	}

	return Super::PromptText();
}

void ATemporaryItemContainer::BeginPlay()
{
	Super::BeginPlay();

	CurrentDurability = MaxDurability;
}
