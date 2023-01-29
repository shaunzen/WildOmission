// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerSaveHandlerComponent.h"
#include "SaveHandler.h"
#include "WildOmissionSaveGame.h"
#include "WildOmission/Player/WildOmissionPlayerController.h"
#include "WildOmission/GameModes/WildOmissionGameMode.h"
#include "TimerManager.h"

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
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	if (WildOmissionPlayerController == nullptr)
	{
		return;
	}

	AddSaveToList(WildOmissionPlayerController->SavePlayer(), PendingSaves);
}

void UPlayerSaveHandlerComponent::Save(TArray<FWildOmissionPlayerSave>& OutUpdatedSavesList)
{
	AddSavesToList(PendingSaves, OutUpdatedSavesList);
	PendingSaves.Empty();
}

void UPlayerSaveHandlerComponent::Load(APlayerController* PlayerController)
{
	ASaveHandler* SaveHandlerOwner = Cast<ASaveHandler>(GetOwner());
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	if (SaveHandlerOwner == nullptr || WildOmissionPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't load the player, SaveHandler or PlayerController was nullptr."));
		return;
	}

	int32 SaveIndex = 0;
	FString PlayerUniqueID = WildOmissionPlayerController->GetUniqueID();

	// Does the player have a pending save that is more up to date than the save file
	if (GetSaveIndexInList(PendingSaves, PlayerUniqueID, SaveIndex))
	{
		WildOmissionPlayerController->LoadPlayerSave(PendingSaves[SaveIndex]);
	}
	// If there is no pending saves does the player exist in the save file
	else if (GetSaveIndexInList(SaveHandlerOwner->GetSaveFile()->PlayerSaves, PlayerUniqueID, SaveIndex))
	{
		WildOmissionPlayerController->LoadPlayerSave(SaveHandlerOwner->GetSaveFile()->PlayerSaves[SaveIndex]);
	}
	// Must be a new player just go ahead and call his spawn function
	else
	{
		WildOmissionPlayerController->Spawn();
	}
}

void UPlayerSaveHandlerComponent::AddAllToPending()
{
	AWildOmissionGameMode* WildOmissionGameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (WildOmissionGameMode == nullptr)
	{
		return;
	}

	for (APlayerController* PlayerController : WildOmissionGameMode->GetAllPlayerControllers())
	{
		AddToPending(PlayerController);
	}
}

void UPlayerSaveHandlerComponent::AddSavesToList(const TArray<FWildOmissionPlayerSave>& InSavesList, TArray<FWildOmissionPlayerSave>& OutSavesList)
{
	for (const FWildOmissionPlayerSave& InSave : InSavesList)
	{
		AddSaveToList(InSave, OutSavesList);
	}
}

void UPlayerSaveHandlerComponent::AddSaveToList(const FWildOmissionPlayerSave& InSave, TArray<FWildOmissionPlayerSave>& OutSavesList)
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

bool UPlayerSaveHandlerComponent::GetSaveIndexInList(const TArray<FWildOmissionPlayerSave>& List, const FString& UniqueID, int32& OutIndex)
{
	int32 Index = 0;
	bool SaveFound = false;
	for (const FWildOmissionPlayerSave& Save : List)
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