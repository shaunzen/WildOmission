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
	// start a timer to gather all saves
	FTimerHandle UpdatePendingTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(UpdatePendingTimerHandle, this, &UPlayerSaveHandlerComponent::UpdatePendingList, 10.0f, true);
}

void UPlayerSaveHandlerComponent::SavePlayers(TArray<FWildOmissionPlayerSave>& OutUpdatedPlayerSaves)
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save players, GameMode was nullptr."));
		return;
	}

	AddPendingSavesToList(OutUpdatedPlayerSaves);
}

void UPlayerSaveHandlerComponent::UpdatePendingList()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create pending player saves, GameMode was nullptr."));
		return;
	}

	CreatePlayerSaves(GameMode->GetAllPlayerControllers(), PendingSaves);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Cyan, FString::Printf(TEXT("%i Players Pending to be saved."), PendingSaves.Num()));
}

void UPlayerSaveHandlerComponent::AddPendingSavesToList(TArray<FWildOmissionPlayerSave>& OutListToAddTo)
{
	for (FWildOmissionPlayerSave& PendingSave : PendingSaves)
	{
		int32 Index = 0;
		if (GetPlayerIndexInList(OutListToAddTo, PendingSave.UniqueID, Index))
		{
			OutListToAddTo[Index] = PendingSave;
		}
		else
		{
			OutListToAddTo.Add(PendingSave);
		}
	}
}

void UPlayerSaveHandlerComponent::LoadPlayer(APlayerController* PlayerController)
{
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	if (WildOmissionPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load player, couldn't cast PlayerController to a WildOmissionPlayerController."));
		return;
	}

	FString PlayerUniqueID = WildOmissionPlayerController->GetUniqueID();
	FWildOmissionPlayerSave PlayerSave;
	if (!RetrivePlayerDataFromSave(PlayerUniqueID, PlayerSave) || PlayerSave.IsAlive == false)
	{
		WildOmissionPlayerController->Spawn();
		return;
	}

	WildOmissionPlayerController->LoadPlayerSave(PlayerSave);
}

void UPlayerSaveHandlerComponent::CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<FWildOmissionPlayerSave>& OutPlayerSaves)
{
	for (APlayerController* PlayerController : PlayerControllersToSave)
	{
		AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
		if (WildOmissionPlayerController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create player saves, could't cast PlayerController to a WildOmissionPlayerController."));
			return;
		}

		int32 SaveIndex = 0;
		if (GetPlayerIndexInList(OutPlayerSaves, WildOmissionPlayerController->GetUniqueID(), SaveIndex))
		{
			OutPlayerSaves[SaveIndex] = WildOmissionPlayerController->SavePlayer();
		}
		else
		{
			OutPlayerSaves.Add(WildOmissionPlayerController->SavePlayer());
		}
	}
}

bool UPlayerSaveHandlerComponent::RetrivePlayerDataFromSave(const FString& PlayerUniqueID, FWildOmissionPlayerSave& OutPlayerSave)
{
	ASaveHandler* OwnerSaveHandler = Cast<ASaveHandler>(GetOwner());
	if (OwnerSaveHandler == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to retrive player data from save, OwnerSaveHandler was nullptr."));
		return false;
	}
	UWildOmissionSaveGame* SaveFile = OwnerSaveHandler->GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrive player data from save, save file was a nullptr."));
		return false;
	}

	int32 PlayerSaveIndex = 0;
	if (!GetPlayerIndexInList(SaveFile->PlayerSaves, PlayerUniqueID, PlayerSaveIndex))
	{
		return false;
	}

	OutPlayerSave = SaveFile->PlayerSaves[PlayerSaveIndex];
	return true;
}

bool UPlayerSaveHandlerComponent::GetPlayerIndexInList(const TArray<FWildOmissionPlayerSave>& List, const FString& PlayerUniqueID, int32& OutIndex)
{
	int32 Index = 0;
	bool PlayerFound = false;
	for (const FWildOmissionPlayerSave& PlayerSave : List)
	{
		if (PlayerSave.UniqueID != PlayerUniqueID)
		{
			++Index;
			continue;
		}
		PlayerFound = true;
		break;
	}

	OutIndex = Index;
	return PlayerFound;
}