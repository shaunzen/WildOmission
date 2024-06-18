// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/PookaPooka.h"
#include "Components/VitalsComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"

static USoundBase* LandSound = nullptr;
static USoundBase* ExplosionSound = nullptr;
static UNiagaraSystem* DeathExplosionEffect = nullptr;

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

	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSoundBlueprint(TEXT("/Game/Weapons/Audio/Explosions/MS_Explosion_Small"));
	if (ExplosionSoundBlueprint.Succeeded())
	{
		ExplosionSound = ExplosionSoundBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DeathExplosionEffectBlueprint(TEXT("/Game/WildOmissionCore/Art/Effects/NS_Explosion"));
	if (DeathExplosionEffectBlueprint.Succeeded())
	{
		DeathExplosionEffect = DeathExplosionEffectBlueprint.Object;
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
	Multi_PlayDeathExplosion();

	const bool DropGold = FMath::RandBool();

	FInventoryItem LootDrop;
	LootDrop.Name = DropGold ? TEXT("gold") : TEXT("metal.refined");
	LootDrop.Quantity = FMath::RandRange(1, 2);
	UInventoryComponent::SpawnWorldItem(GetWorld(), LootDrop, this);

	Super::HandleDeath();
}

void APookaPooka::Multi_PlayLandSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandSound, this->GetActorLocation());
}

void APookaPooka::Multi_PlayDeathExplosion_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, this->GetActorLocation());
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathExplosionEffect, this->GetActorLocation());
}
