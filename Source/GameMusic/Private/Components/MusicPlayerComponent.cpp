// Copyright Telephone Studios. All Rights Reserved.


#include "Components/MusicPlayerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UMusicPlayerComponent::UMusicPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	TimeBetweenSongs = 300.0f;
	MusicCue = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundBase> MusicCueObject(TEXT("/Game/GameMusic/Audio/Music_Cue"));
	if (MusicCueObject.Succeeded())
	{
		MusicCue = MusicCueObject.Object;
	}
}

// Called when the game starts
void UMusicPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerDelegate PlayMusicTimerDelegate;
	PlayMusicTimerDelegate.BindUObject(this, &UMusicPlayerComponent::PlayMusicTrack);
	World->GetTimerManager().SetTimer(PlayMusicTimerHandle, PlayMusicTimerDelegate, TimeBetweenSongs, true);
	PlayMusicTrack();
}

void UMusicPlayerComponent::PlayMusicTrack()
{
	UWorld* World = GetWorld();
	bool ShouldPlay = UKismetMathLibrary::RandomBoolWithWeight(0.5f);
	if (!ShouldPlay || World == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(World, MusicCue);
}
