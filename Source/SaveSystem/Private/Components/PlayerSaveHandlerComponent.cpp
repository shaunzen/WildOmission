// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PlayerSaveHandlerComponent.h"
#include "WildOmissionSaveGame.h"
#include "Interfaces/SavablePlayer.h"
#include "Actors/SaveHandler.h"
#include "Structs/PlayerSave.h"
#include "Log.h"

// Sets default values for this component's properties
UPlayerSaveHandlerComponent::UPlayerSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UPlayerSaveHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle UpdatePendingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(UpdatePendingTimerHandle, this, &UPlayerSaveHandlerComponent::AddAllToPending, 60.0f, true);
}

void UPlayerSaveHandlerComponent::AddToPending(APlayerController* PlayerController)
{
	ISavablePlayer* SavablePlayer = Cast<ISavablePlayer>(PlayerController);
	if (SavablePlayer == nullptr)
	{
		return;
	}

	AddSaveToList(SavablePlayer->SavePlayer(), PendingSaves);
}

void UPlayerSaveHandlerComponent::Save(TArray<FPlayerSave>& OutUpdatedSavesList)
{
	for (const FPlayerSave& InSave : PendingSaves)
	{
		AddSaveToList(InSave, OutUpdatedSavesList);
	}

	PendingSaves.Empty();
}

void UPlayerSaveHandlerComponent::Load(APlayerController* PlayerController)
{
	ASaveHandler* SaveHandlerOwner = Cast<ASaveHandler>(GetOwner());
	ISavablePlayer* SavablePlayer = Cast<ISavablePlayer>(PlayerController);
	if (SaveHandlerOwner == nullptr || SavablePlayer == nullptr)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Couldn't load the player, SaveHandler or PlayerController was nullptr."));
		return;
	}

	FString PlayerUniqueID = SavablePlayer->GetUniqueID();
	bool PlayerIsHost = SavablePlayer->IsHost();

	if (PlayerIsHost)
	{
		const int32 SaveIndex = GetHostSaveIndexFromList(SaveHandlerOwner->GetSaveFile()->PlayerSaves);
		if (SaveIndex == INDEX_NONE)
		{
			return;
		}

		SavablePlayer->LoadPlayerSave(SaveHandlerOwner->GetSaveFile()->PlayerSaves[SaveIndex]);
		return;
	}

	// Does the player have a pending save that is more up to date than the save file
	if (const int32 SaveIndex = GetSaveIndexFromList(PendingSaves, PlayerUniqueID) != INDEX_NONE)
	{
		SavablePlayer->LoadPlayerSave(PendingSaves[SaveIndex]);
	}
	// If there is no pending saves, does the player exist in the save file
	else if (const int32 SaveIndex = GetSaveIndexFromList(SaveHandlerOwner->GetSaveFile()->PlayerSaves, PlayerUniqueID) != INDEX_NONE)
	{
		SavablePlayer->LoadPlayerSave(SaveHandlerOwner->GetSaveFile()->PlayerSaves[SaveIndex]);
	}
}

void UPlayerSaveHandlerComponent::AddAllToPending()
{
	for (APlayerController* PlayerController : GetAllPlayerControllers())
	{
		AddToPending(PlayerController);
	}
}

void UPlayerSaveHandlerComponent::AddSaveToList(const FPlayerSave& InSave, TArray<FPlayerSave>& OutSavesList)
{
	if (const int32 SaveIndex = GetSaveIndexFromList(OutSavesList, InSave.UniqueID) != INDEX_NONE)
	{
		OutSavesList[SaveIndex] = InSave;
	}
	else
	{
		OutSavesList.Add(InSave);
	}
}

int32 UPlayerSaveHandlerComponent::GetSaveIndexFromList(const TArray<FPlayerSave>& SaveList, const FString& UniqueID) const
{
	int32 Index = 0;
	for (const FPlayerSave& Save : SaveList)
	{
		if (Save.UniqueID != UniqueID)
		{
			++Index;
			continue;
		}

		return Index;
	}

	return -1;
}

int32 UPlayerSaveHandlerComponent::GetHostSaveIndexFromList(const TArray<FPlayerSave>& SaveList) const
{
	int32 Index = 0;
	for (const FPlayerSave& Save : SaveList)
	{
		if (Save.IsHost == false)
		{
			++Index;
			continue;
		}

		return Index;
	}

	return -1;
}

TArray<APlayerController*> UPlayerSaveHandlerComponent::GetAllPlayerControllers()
{
	TArray<APlayerController*> PlayerControllers;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		PlayerControllers.Add(Iterator->Get());
	}

	return PlayerControllers;
}