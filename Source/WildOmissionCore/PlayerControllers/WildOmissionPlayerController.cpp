// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"
#include "Interfaces/RequiredForLoad.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Components/VitalsComponent.h"
#include "GameFramework/PlayerState.h"
#include "WildOmissionCore/GameModes/WildOmissionGameMode.h"
#include "Actors/SaveHandler.h"
#include "Components/PlayerSaveHandlerComponent.h"
#include "WildOmissionCore/UI/Player/PlayerHUDWidget.h"
#include "WildOmissionCore/UI/Player/DeathMenuWidget.h"
#include "UI/GameplayMenuWidget.h"
#include "WildOmissionCore/WildOmissionStatics.h"
#include "WildOmissionCore/WildOmissionGameState.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "UObject/ConstructorHelpers.h"

AWildOmissionPlayerController::AWildOmissionPlayerController()
{
	bIsStillLoading = true;
	NumRequiredActorsForLoad = 0;

	static ConstructorHelpers::FClassFinder<UDeathMenuWidget> DeathMenuWidgetBlueprint(TEXT("/Game/WildOmissionCore/UI/Player/WBP_DeathMenu"));
	if (DeathMenuWidgetBlueprint.Succeeded())
	{
		DeathMenuWidgetClass = DeathMenuWidgetBlueprint.Class;
	}
}

void AWildOmissionPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

FPlayerSave AWildOmissionPlayerController::SavePlayer()
{
	FPlayerSave PlayerSave;
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
	PlayerSave.IsHost = IsHost();
	
	PlayerSave.Vitals.Health = WildOmissionCharacter->GetVitalsComponent()->GetHealth();
	PlayerSave.Vitals.Hunger = WildOmissionCharacter->GetVitalsComponent()->GetHunger();
	PlayerSave.Vitals.Thirst = WildOmissionCharacter->GetVitalsComponent()->GetThirst();

	PlayerSave.Inventory.ByteData = WildOmissionCharacter->GetInventoryComponent()->Save();
	
	PlayerSave.SelectedItemByteData = WildOmissionCharacter->GetInventoryManipulatorComponent()->GetSelectedItemAsByteData();

	return PlayerSave;
}

void AWildOmissionPlayerController::LoadPlayerSave(const FPlayerSave& PlayerSave)
{
	StoredPlayerSave = PlayerSave;
}

FString AWildOmissionPlayerController::GetUniqueID()
{
	FString ID = TEXT("");

	if (PlayerState)
	{
		ID = PlayerState->GetUniqueId().ToString();
	}

	return ID;
}

bool AWildOmissionPlayerController::IsHost()
{
	return GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy;
}

void AWildOmissionPlayerController::SendMessage(APlayerState* Sender, const FString& Message)
{
	Server_SendChatMessage(Sender, Message);
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

bool AWildOmissionPlayerController::IsEditorPlayer() const
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

void AWildOmissionPlayerController::Client_SetNumRequiredActors_Implementation(const int32& InNum)
{
	UE_LOG(LogPlayerController, Verbose, TEXT("NumRequiredActorsSet: %i"), InNum);
	NumRequiredActorsForLoad = InNum;
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

void AWildOmissionPlayerController::Server_SendChatMessage_Implementation(APlayerState* Sender, const FString& Message)
{
	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr || Sender == nullptr)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Failed to send chat message, couldn't get state."));
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
		UpdateClientRequiredActorCountTimerDelegate.BindUObject(this, &AWildOmissionPlayerController::CountRequiredActorsAndSendToClient);
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

void AWildOmissionPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (!HasAuthority())
	{
		return;
	}

	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(aPawn);

	if (WildOmissionCharacter == nullptr || bIsStillLoading == false || StoredPlayerSave.IsAlive == false)
	{
		return;
	}

	WildOmissionCharacter->SetActorLocation(StoredPlayerSave.WorldLocation);

	WildOmissionCharacter->GetVitalsComponent()->SetHealth(StoredPlayerSave.Vitals.Health);
	WildOmissionCharacter->GetVitalsComponent()->SetHunger(StoredPlayerSave.Vitals.Hunger);
	WildOmissionCharacter->GetVitalsComponent()->SetThirst(StoredPlayerSave.Vitals.Thirst);

	WildOmissionCharacter->GetInventoryComponent()->Load(StoredPlayerSave.Inventory.ByteData);
	WildOmissionCharacter->GetInventoryManipulatorComponent()->LoadSelectedItemFromByteDataAndDropInWorld(StoredPlayerSave.SelectedItemByteData);
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

void AWildOmissionPlayerController::CountRequiredActorsAndSendToClient()
{
	int32 NewRequiredAmount = IRequiredForLoad::GetNumRequiredActorsInWorld(GetWorld());
	Client_SetNumRequiredActors(NewRequiredAmount);
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

	// Make Sure to Close Pause Menu So it Doesn't Get Stuck on Screen!
	UWildOmissionGameInstance* WOGameInstance = UWildOmissionGameInstance::GetWildOmissionGameInstance(GetWorld());
	if (WOGameInstance && WOGameInstance->GetGameplayMenuWidget() != nullptr)
	{
		WOGameInstance->GetGameplayMenuWidget()->Teardown();
	}

	UE_LOG(LogPlayerController, Verbose, TEXT("Bringing Up Death Menu."));

	UDeathMenuWidget* DeathMenu = CreateWidget<UDeathMenuWidget>(this, DeathMenuWidgetClass);
	if (DeathMenu == nullptr)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Failed to show death menu."));
		return;
	}

	DeathMenu->Show();
}