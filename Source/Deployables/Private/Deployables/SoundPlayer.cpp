// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/SoundPlayer.h"
#include "Kismet/GameplayStatics.h"

ASoundPlayer::ASoundPlayer()
{
	Prompt = TEXT("Play Sound");
	Sound = nullptr;
}

void ASoundPlayer::Interact(AActor* Interactor)
{
	Multi_PlaySound();
}

FString ASoundPlayer::PromptText()
{
	return Prompt;
}

void ASoundPlayer::Multi_PlaySound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, this->GetActorLocation());
}
