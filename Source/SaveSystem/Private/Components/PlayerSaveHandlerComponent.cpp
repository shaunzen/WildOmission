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
	AddSavesToList(PendingSaves, OutUpdatedSavesList);
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
	FString PlayerUniqueID = SavablePlayer->GetUniqueID();

	// Does the player have a pending save that is more up to date than the save file
	if (GetSaveIndexInList(PendingSaves, PlayerUniqueID, SaveIndex))
	{
		SavablePlayer->LoadPlayerSave(PendingSaves[SaveIndex]);
	}
	// If there is no pending saves does the player exist in the save file
	else if (GetSaveIndexInList(SaveHandlerOwner->GetSaveFile()->PlayerSaves, PlayerUniqueID, SaveIndex))
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

void UPlayerSaveHandlerComponent::AddSavesToList(const TArray<FPlayerSave>& InSavesList, TArray<FPlayerSave>& OutSavesList)
{
	for (const FPlayerSave& InSave : InSavesList)
	{
		AddSaveToList(InSave, OutSavesList);
	}
}

void UPlayerSaveHandlerComponent::AddSaveToList(const FPlayerSave& InSave, TArray<FPlayerSave>& OutSavesList)
{
	int32 SaveIndex = 0;
	if (GetSaveIndexInList(OutSavesList, InSave.UniqueID, SaveIndex))
	{
		OutSavesList[SaveIndex] = InSave;
	}
	else
	{
		OutSavesList.Add(InSave);
	}
}

bool UPlayerSaveHandlerComponent::GetSaveIndexInList(const TArray<FPlayerSave>& List, const FString& UniqueID, int32& OutIndex)
{
	int32 Index = 0;
	bool SaveFound = false;
	for (const FPlayerSave& Save : List)
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

TArray<APlayerController*> UPlayerSaveHandlerComponent::GetAllPlayerControllers()
{
	TArray<APlayerController*> PlayerControllers;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		PlayerControllers.Add(Iterator->Get());
	}

	return PlayerControllers;
}