// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PlayerSaveHandlerComponent.h"
#include "WildOmissionSaveGame.h"
#include "Interfaces/SavablePlayer.h"
#include "SaveHandler.h"
#include "Structs/PlayerSaveData.h"
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

void UPlayerSaveHandlerComponent::Save(TArray<FPlayerSaveData>& OutUpdatedSavesList)
{
	for (const FPlayerSaveData& InSave : PendingSaves)
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

	UWildOmissionSaveGame* SaveFile = SaveHandlerOwner->GetSaveFile();
	if (SaveFile == nullptr)
	{
		return;
	}

	// Attempt host parody
	if (PlayerIsHost && FindHostSaveIndexInList(SaveFile->PlayerSaveData, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(SaveFile->PlayerSaveData[SaveIndex]);
		SaveFile->PlayerSaveData[SaveIndex].IsHost = false;
		SaveHandlerOwner->UpdateSaveFile(SaveFile);
		return;
	}

	// Find existing save in the pending list
	if (FindSaveIndexInList(PendingSaves, PlayerUniqueID, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(PendingSaves[SaveIndex]);
	}
	// Find existing save in the save file
	else if (FindSaveIndexInList(SaveFile->PlayerSaveData, PlayerUniqueID, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(SaveFile->PlayerSaveData[SaveIndex]);
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

void UPlayerSaveHandlerComponent::AddSaveToList(const FPlayerSaveData& InSave, TArray<FPlayerSaveData>& OutSavesList)
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

bool UPlayerSaveHandlerComponent::FindSaveIndexInList(const TArray<FPlayerSaveData>& SaveList, const FString& UniqueID, int32& OutIndex) const
{
	int32 Index = 0;
	bool SaveFound = false;
	for (const FPlayerSaveData& Save : SaveList)
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

bool UPlayerSaveHandlerComponent::FindHostSaveIndexInList(const TArray<FPlayerSaveData>& SaveList, int32& OutIndex) const
{
	int32 Index = 0;
	bool SaveFound = false;
	for (const FPlayerSaveData& Save : SaveList)
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