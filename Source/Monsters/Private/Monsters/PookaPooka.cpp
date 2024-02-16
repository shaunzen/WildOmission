// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/PookaPooka.h"
#include "Components/VitalsComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// TODO would be cool if when they land it caused nearby players cameras to shake
// TODO they shoot "Pook" at you and the sound is makes sounds like "Pook"

// TODO they explode on death

static USoundBase* LandSound = nullptr;

APookaPooka::APookaPooka()
{
	MaxTimeBetweenIdleSoundSeconds = 10.0f;
	MinTimeBetweenIdleSoundSeconds = 5.0f;

	DefaultWalkSpeed = 400.0f;

	ItemToEquip.Name = TEXT("cannon.pooka");
	ItemToEquip.Quantity = 1;

	static ConstructorHelpers::FObjectFinder<USoundBase> PookaPookaIdle(TEXT("/Game/Monsters/Audio/PookaPooka/PookaPooka_Idle_Cue"));
	if (PookaPookaIdle.Succeeded())
	{
		IdleSound = PookaPookaIdle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> LandSoundCue(TEXT("/Game/Monsters/Audio/PookaPooka/PookaPooka_Land_Cue"));
	if (LandSoundCue.Succeeded())
	{
		LandSound = LandSoundCue.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> PookaPookaHurt(TEXT("/Game/Monsters/Audio/PookaPooka/PookaPooka_Hurt_Cue"));
	if (PookaPookaHurt.Succeeded() && VitalsComponent)
	{
		VitalsComponent->SetHurtSound(PookaPookaHurt.Object);
	}
}

void APookaPooka::SetIsChasing(bool IsChasing)
{
	IdleSoundsEnabled = IsChasing;
}

void APookaPooka::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	const float CurrentVelocity = FMath::Abs(this->GetVelocity().Length());
	if (CurrentVelocity < 10.0f)
	{
		return;
	}

	this->Jump();
}

void APookaPooka::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	Multi_PlayLandSound();
}

void APookaPooka::HandleDeath()
{
	Super::HandleDeath();

	FInventoryItem LootDrop;
	LootDrop.Name = TEXT("metal.refined");
	LootDrop.Quantity = FMath::RandRange(1, 3);
	UInventoryComponent::SpawnWorldItem(GetWorld(), LootDrop, this);
}

void APookaPooka::Multi_PlayLandSound_Implementation()
{
	if (LandSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandSound, this->GetActorLocation());
}
