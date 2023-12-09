// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"
#include "Interfaces/RequiredForLoad.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Components/VitalsComponent.h"
#include "GameFramework/PlayerState.h"
#include "WildOmissionCore/GameModes/WildOmissionGameMode.h"
#include "SaveHandler.h"
#include "GameChatHandler.h"
#include "Components/PlayerSaveHandlerComponent.h"
#include "WildOmissionCore/UI/Player/PlayerHUDWidget.h"
#include "WildOmissionCore/UI/Player/DeathMenuWidget.h"
#include "UI/GameplayMenuWidget.h"
#include "Components/MusicPlayerComponent.h"
#include "WildOmissionCore/WildOmissionStatics.h"
#include "WildOmissionCore/WildOmissionGameState.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

AWildOmissionPlayerController::AWildOmissionPlayerController()
{
	bIsStillLoading = true;
	NumRequiredActorsForLoad = 0;

	BedUniqueID = -1;

	MusicPlayerComponent = nullptr;

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

void AWildOmissionPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWildOmissionPlayerController, BedUniqueID, COND_OwnerOnly);
}

FPlayerSaveData AWildOmissionPlayerController::SavePlayer()
{
	FPlayerSaveData PlayerSaveData;

	if (HasAuthority() == false)
	{
		return PlayerSaveData;
	}

	APlayerState* CurrentPlayerState = PlayerState.Get();
	if (CurrentPlayerState == nullptr)
	{
		return PlayerSaveData;
	}

	PlayerSaveData.UniqueID = CurrentPlayerState->GetUniqueId().ToString();
	PlayerSaveData.BedUniqueID = BedUniqueID;
	PlayerSaveData.NewPlayer = false;
	
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetCharacter());
	if (WildOmissionCharacter == nullptr)
	{
		PlayerSaveData.IsAlive = false;
		return PlayerSaveData;
	}

	PlayerSaveData.WorldLocation = WildOmissionCharacter->GetActorLocation();
	
	PlayerSaveData.IsAlive = true;
	PlayerSaveData.IsHost = IsHost();

	PlayerSaveData.Vitals.Health = WildOmissionCharacter->GetVitalsComponent()->GetHealth();
	PlayerSaveData.Vitals.Hunger = WildOmissionCharacter->GetVitalsComponent()->GetHunger();
	PlayerSaveData.Vitals.Thirst = WildOmissionCharacter->GetVitalsComponent()->GetThirst();

	PlayerSaveData.Inventory.ByteData = WildOmissionCharacter->GetInventoryComponent()->Save();
	
	PlayerSaveData.SelectedItemByteData = WildOmissionCharacter->GetInventoryManipulatorComponent()->GetSelectedItemAsByteData();

	return PlayerSaveData;
}

void AWildOmissionPlayerController::LoadPlayerSave(const FPlayerSaveData& SaveData)
{
	BedUniqueID = SaveData.BedUniqueID;

	StoredPlayerSaveData = SaveData;
}

bool AWildOmissionPlayerController::IsStillLoading() const
{
	return bIsStillLoading;
}

FString AWildOmissionPlayerController::GetUniqueID() const
{
	FString ID = TEXT("");

	if (PlayerState)
	{
		ID = PlayerState->GetUniqueId().ToString();
	}

	return ID;
}

bool AWildOmissionPlayerController::IsHost() const
{
	return GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy;
}

void AWildOmissionPlayerController::SendMessage(APlayerState* Sender, const FString& Message)
{
	Server_SendMessage(Sender, Message);
}

void AWildOmissionPlayerController::SetBedUniqueID(const int32& InUniqueID)
{
	BedUniqueID = InUniqueID;
}

int32 AWildOmissionPlayerController::GetBedUniqueID() const
{
	return BedUniqueID;
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
	if (GameInstance)
	{
		GameInstance->StartLoading();
		GameInstance->SetLoadingSubtitle(TEXT("Loading world state."));
	}

	if (MusicPlayerComponent == nullptr)
	{
		MusicPlayerComponent = NewObject<UMusicPlayerComponent>(this, UMusicPlayerComponent::StaticClass(), TEXT("MusicPlayerComponent"));
		if (MusicPlayerComponent)
		{
			MusicPlayerComponent->RegisterComponent();
		}
	}

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

	if (WildOmissionCharacter == nullptr || bIsStillLoading == false || StoredPlayerSaveData.IsAlive == false || StoredPlayerSaveData.NewPlayer == true)
	{
		return;
	}

	WildOmissionCharacter->SetActorLocation(StoredPlayerSaveData.WorldLocation);
	
	WildOmissionCharacter->GetVitalsComponent()->SetHealth(StoredPlayerSaveData.Vitals.Health);
	WildOmissionCharacter->GetVitalsComponent()->SetHunger(StoredPlayerSaveData.Vitals.Hunger);
	WildOmissionCharacter->GetVitalsComponent()->SetThirst(StoredPlayerSaveData.Vitals.Thirst);

	WildOmissionCharacter->GetInventoryComponent()->Load(StoredPlayerSaveData.Inventory.ByteData);
	WildOmissionCharacter->GetInventoryManipulatorComponent()->LoadSelectedItemFromByteDataAndDropInWorld(StoredPlayerSaveData.SelectedItemByteData);

	StoredPlayerSaveData = FPlayerSaveData();
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
	if (GameInstance == nullptr)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Couldnt Stop Loading, GameInstance returned a nullptr."));
		return;
	}
	GameInstance->StopLoading();
}

void AWildOmissionPlayerController::Server_SendMessage_Implementation(APlayerState* Sender, const FString& Message)
{
	AGameChatHandler* ChatHandler = AGameChatHandler::GetGameChatHandler();
	if (ChatHandler == nullptr)
	{
		return;
	}

	ChatHandler->SendMessage(Sender, Message, false);
}

void AWildOmissionPlayerController::Server_Spawn_Implementation()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode && (StoredPlayerSaveData.IsAlive || StoredPlayerSaveData.NewPlayer))
	{
		GameMode->SpawnHumanForController(this);
	}
	else
	{
		Client_ShowDeathMenu();
	}

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