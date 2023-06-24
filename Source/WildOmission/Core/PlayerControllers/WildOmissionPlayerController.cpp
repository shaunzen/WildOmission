// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Core/Interfaces/RequiredForLoad.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "GameFramework/PlayerState.h"
#include "WildOmission/Core/GameModes/WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Core/SaveSystem/PlayerSaveHandlerComponent.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "WildOmission/UI/Player/DeathMenuWidget.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "WildOmission/Core/WildOmissionGameState.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "UObject/ConstructorHelpers.h"

AWildOmissionPlayerController::AWildOmissionPlayerController()
{
	ConstructorHelpers::FClassFinder<UDeathMenuWidget> DeathMenuWidgetBlueprint(TEXT("/Game/WildOmission/UI/Player/WBP_DeathMenu"));
	if (DeathMenuWidgetBlueprint.Succeeded())
	{
		DeathMenuWidgetClass = DeathMenuWidgetBlueprint.Class;
	}

	bIsStillLoading = true;
}

void AWildOmissionPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

FWildOmissionPlayerSave AWildOmissionPlayerController::SavePlayer()
{
	FWildOmissionPlayerSave PlayerSave;
	if (HasAuthority() == false)
	{
		return PlayerSave;
	}

	APlayerState* CurrentPlayerState = PlayerState.Get();
	if (CurrentPlayerState == nullptr)
	{
		return PlayerSave;
	}
	PlayerSave.UniqueID = CurrentPlayerState->GetUniqueId().ToString();
	
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetCharacter());
	if (WildOmissionCharacter == nullptr)
	{
		PlayerSave.IsAlive = false;
		return PlayerSave;
	}


	PlayerSave.WorldLocation = WildOmissionCharacter->GetActorLocation();
	PlayerSave.IsAlive = true;
	
	PlayerSave.Vitals.Health = WildOmissionCharacter->GetVitalsComponent()->GetHealth();
	PlayerSave.Vitals.Hunger = WildOmissionCharacter->GetVitalsComponent()->GetHunger();
	PlayerSave.Vitals.Thirst = WildOmissionCharacter->GetVitalsComponent()->GetThirst();

	PlayerSave.Inventory = WildOmissionCharacter->GetInventoryComponent()->Save();
	PlayerSave.SelectedItem = WildOmissionCharacter->GetInventoryManipulatorComponent()->GetSelectedItem();

	return PlayerSave;
}

void AWildOmissionPlayerController::LoadPlayerSave(const FWildOmissionPlayerSave& PlayerSave)
{
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetCharacter());

	if (WildOmissionCharacter == nullptr)
	{
		return;
	}

	WildOmissionCharacter->SetActorLocation(PlayerSave.WorldLocation);
	
	WildOmissionCharacter->GetVitalsComponent()->SetHealth(PlayerSave.Vitals.Health);
	WildOmissionCharacter->GetVitalsComponent()->SetHunger(PlayerSave.Vitals.Hunger);
	WildOmissionCharacter->GetVitalsComponent()->SetThirst(PlayerSave.Vitals.Thirst);

	WildOmissionCharacter->GetInventoryComponent()->Load(PlayerSave.Inventory);

	if (PlayerSave.SelectedItem.Quantity > 0)
	{
		UInventoryComponent::SpawnWorldItem(GetWorld(), PlayerSave.SelectedItem, WildOmissionCharacter);
	}
}

void AWildOmissionPlayerController::Save()
{
	Server_AddToPendingSaves();

	if (HasAuthority() == false)
	{
		return;
	}
	
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	GameMode->SaveGame();
}

void AWildOmissionPlayerController::Client_SetNumRequiredActors_Implementation(const int32& InNum)
{
	NumRequiredActorsForLoad = InNum;
}

FString AWildOmissionPlayerController::GetUniqueID()
{
	FString ID = FString("");
	
	if (PlayerState)
	{
		ID = PlayerState->GetUniqueId().ToString();
	}

	return ID;
}

void AWildOmissionPlayerController::Server_SendChatMessage_Implementation(APlayerState* Sender, const FString& Message)
{
	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr || Sender == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to send chat message, couldn't get state."));
		return;
	}

	GameState->AddChatMessage(Sender, Message);
}

void AWildOmissionPlayerController::Server_KillThisPlayer_Implementation()
{
	// Get pawn
	APawn* OurPawn = GetPawn();
	if (OurPawn == nullptr)
	{
		return;
	}

	// Get vitals from pawn
	UVitalsComponent* PawnVitals = OurPawn->FindComponentByClass<UVitalsComponent>();
	if (PawnVitals == nullptr)
	{
		return;
	}

	// Set health to zero
	PawnVitals->SetHealth(0.0f);
}

void AWildOmissionPlayerController::Server_AddToPendingSaves_Implementation()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	GameMode->GetSaveHandler()->GetPlayerHandler()->AddToPending(this);
}

//*****************************
// Console functions

void AWildOmissionPlayerController::Kill()
{
	Server_KillThisPlayer();
}

void AWildOmissionPlayerController::LogLocalInventoryContents()
{
	if (!IsLocalController())
	{
		return;
	}
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetPawn());
	if (WildOmissionCharacter == nullptr)
	{
		return;
	}

	// Get their inventory components
	for (const FInventoryItem& ItemData : GetPawn()->FindComponentByClass<UPlayerInventoryComponent>()->GetContents()->Contents)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Item: %s, Quantity: %i"), *ItemData.Name.ToString(), ItemData.Quantity));
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: %s"), *WildOmissionCharacter->GetActorNameOrLabel()));
}

void AWildOmissionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FTimerDelegate UpdateClientRequiredActorCountTimerDelegate;
		UpdateClientRequiredActorCountTimerDelegate.BindUFunction(this, "Client_SetNumRequiredActors", IRequiredForLoad::GetNumRequiredActorsInWorld(GetWorld()));
		GetWorld()->GetTimerManager().SetTimer(UpdateClientRequiredActorCountTimerHandle, UpdateClientRequiredActorCountTimerDelegate, 10.0f, true);
	}

	if (!IsLocalController())
	{
		return;
	}

	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->StartLoading();
	GameInstance->SetLoadingSubtitle(FString("Loading world state."));

	if (HasAuthority())
	{
		FTimerHandle LoadTimerHandle;
		FTimerDelegate LoadTimerDelegate;
		LoadTimerDelegate.BindUObject(this, &AWildOmissionPlayerController::StopLoading);
		GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, LoadTimerDelegate, 1.0f, false);
	}
	else
	{
		FTimerDelegate ValidateWorldStateTimerDelegate;
		ValidateWorldStateTimerDelegate.BindUObject(this, &AWildOmissionPlayerController::ValidateWorldState);
		GetWorld()->GetTimerManager().SetTimer(ValidateWorldStateTimerHandle, ValidateWorldStateTimerDelegate, 2.0f, true);
	}
}

void AWildOmissionPlayerController::ValidateWorldState()
{
	
	if (IRequiredForLoad::GetNumRequiredActorsInWorld(GetWorld()) < NumRequiredActorsForLoad)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(ValidateWorldStateTimerHandle);
	StopLoading();
}

void AWildOmissionPlayerController::StopLoading()
{
	Server_Spawn();
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->StopLoading();
}

void AWildOmissionPlayerController::Server_Spawn_Implementation()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->SpawnHumanForController(this);

	if (bIsStillLoading == true)
	{
		OnFinishedLoading.Broadcast(this);
		bIsStillLoading = false;
		GetWorld()->GetTimerManager().ClearTimer(UpdateClientRequiredActorCountTimerHandle);
	}
}

void AWildOmissionPlayerController::Client_ShowDeathMenu_Implementation()
{
	if (!IsLocalController())
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Showing Death Menu."));

	UDeathMenuWidget* DeathMenu = CreateWidget<UDeathMenuWidget>(this, DeathMenuWidgetClass);
	if (DeathMenu == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to show death menu."));
		return;
	}

	DeathMenu->Show();
}