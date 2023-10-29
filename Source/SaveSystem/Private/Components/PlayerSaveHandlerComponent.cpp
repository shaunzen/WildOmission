// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PlayerSaveHandlerComponent.h"
#include "WildOmissionSaveGame.h"
#include "Interfaces/SavablePlayer.h"
#include "SaveHandler.h"
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

	int32 SaveIndex = 0;
	const FString PlayerUniqueID = SavablePlayer->GetUniqueID();
	const bool PlayerIsHost = SavablePlayer->IsHost();

	if (PlayerIsHost && FindHostSaveIndexInList(SaveHandlerOwner->GetSaveFile()->PlayerSaves, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(SaveHandlerOwner->GetSaveFile()->PlayerSaves[SaveIndex]);
		return;
	}

	// Does the player have a pending save that is more up to date than the save file
	if (FindSaveIndexInList(PendingSaves, PlayerUniqueID, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(PendingSaves[SaveIndex]);
	}
	// If there is no pending saves, does the player exist in the save file
	else if (FindSaveIndexInList(SaveHandlerOwner->GetSaveFile()->PlayerSaves, PlayerUniqueID, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(SaveHandlerOwner->GetSaveFile()->PlayerSaves[SaveIndex]);
	}
}

void UPlayerSaveHandlerComponent::AddAllToPending()
{
	for (APlayerController* PlayerController : GetAllPlayerControllers())
	{
		ISavablePlayer* SavablePlayer = Cast<ISavablePlayer>(PlayerController);
		if (SavablePlayer == nullptr || SavablePlayer->IsStillLoading())
		{
			continue;
		}

		AddToPending(PlayerController);
	}
}

void UPlayerSaveHandlerComponent::AddSaveToList(const FPlayerSave& InSave, TArray<FPlayerSave>& OutSavesList)
{
	int32 SaveIndex = 0;
	if (FindSaveIndexInList(OutSavesList, InSave.UniqueID, SaveIndex))
	{
		OutSavesList[SaveIndex] = InSave;
	}
	else
	{
		OutSavesList.Add(InSave);
	}
}

bool UPlayerSaveHandlerComponent::FindSaveIndexInList(const TArray<FPlayerSave>& SaveList, const FString& UniqueID, int32& OutIndex) const
{
	int32 Index = 0;
	bool SaveFound = false;
	for (const FPlayerSave& Save : SaveList)
	{
		if (Save.UniqueID != UniqueID)
		{
			++Index;
			continue;
		}

		SaveFound = true;
		break;
	}

	OutIndex = Index;
	return SaveFound;
}

bool UPlayerSaveHandlerComponent::FindHostSaveIndexInList(const TArray<FPlayerSave>& SaveList, int32& OutIndex) const
{
	int32 Index = 0;
	bool SaveFound = false;
	for (const FPlayerSave& Save : SaveList)
	{
		if (Save.IsHost == false)
		{
			++Index;
			continue;
		}

		SaveFound = true;
		break;
	}

	OutIndex = Index;
	return SaveFound;
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