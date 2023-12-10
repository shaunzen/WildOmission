// Copyright Telephone Studios. All Rights Reserved.


#include "Components/MusicPlayerComponent.h"
#include "TimeOfDayManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UMusicPlayerComponent::UMusicPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	TimeBetweenSongs = 300.0f;
	
	static ConstructorHelpers::FObjectFinder<USoundBase> MusicCueObject(TEXT("/Game/GameMusic/Audio/Music_Cue"));
	if (AudioComponent && MusicCueObject.Succeeded())
	{
		AudioComponent->SetSound(MusicCueObject.Object);
		AudioComponent->bIsMusic = true;
	}
}

// Called when the game starts
void UMusicPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld())
	{
		return;
	}

	ATimeOfDayManager* TimeOfDayManager = ATimeOfDayManager::GetTimeOfDayManager();
	if (TimeOfDayManager == nullptr)
	{
		FTimerHandle PlayMusicTimerHandle;
		FTimerDelegate PlayMusicTimerDelegate;
		PlayMusicTimerDelegate.BindUObject(this, &UMusicPlayerComponent::PlayMusicTrack);
		World->GetTimerManager().SetTimer(PlayMusicTimerHandle, PlayMusicTimerDelegate, TimeBetweenSongs, true);
		PlayMusicTrack();
		return;
	}

	TimeOfDayManager->OnTimeSunrise.AddDynamic(this, &UMusicPlayerComponent::PlayMusicTrack);
	TimeOfDayManager->OnTimeNoon.AddDynamic(this, &UMusicPlayerComponent::PlayMusicTrack);
	TimeOfDayManager->OnTimeSunset.AddDynamic(this, &UMusicPlayerComponent::PlayMusicTrack);
	TimeOfDayManager->OnTimeMidnight.AddDynamic(this, &UMusicPlayerComponent::PlayMusicTrack);
}

void UMusicPlayerComponent::PlayMusicTrack()
{
	UWorld* World = GetWorld();
	bool ShouldPlay = UKismetMathLibrary::RandomBoolWithWeight(0.5f) && !AudioComponent->IsPlaying();
	if (!ShouldPlay || World == nullptr)
	{
		return;
	}

	AudioComponent->Play();
}
