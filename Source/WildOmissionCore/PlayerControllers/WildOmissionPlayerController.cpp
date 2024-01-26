// Copyright Telephone Studios. All Rights Reserved.

// Player Controller Core
#include "WildOmissionPlayerController.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"
#include "WildOmissionCore/Characters/WildOmissionSpectatorPawn.h"
#include "GameFramework/PlayerState.h"

// Wild Omission Components
#include "Components/VitalsComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/PlayerSaveManagerComponent.h"
#include "Components/MusicPlayerComponent.h"

// Wild Omission Stuff
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "WildOmissionCore/GameModes/WildOmissionGameMode.h"
#include "SaveManager.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "GameChatManager.h"
#include "UI/GameplayMenuWidget.h"
#include "WildOmissionCore/UI/Player/DeathMenuWidget.h"

// Engine Stuff
#include "Net/UnrealNetwork.h"

AWildOmissionPlayerController::AWildOmissionPlayerController()
{
	bIsStillLoading = true;
	SpawnChunk = FIntVector2();

	BedUniqueID = -1;
	BedWorldLocation = FVector::ZeroVector;

	MusicPlayerComponent = nullptr;

	static ConstructorHelpers::FClassFinder<UDeathMenuWidget> DeathMenuWidgetBlueprint(TEXT("/Game/WildOmissionCore/UI/Player/WBP_DeathMenu"));
	if (DeathMenuWidgetBlueprint.Succeeded())
	{
		DeathMenuWidgetClass = DeathMenuWidgetBlueprint.Class;
	}
}

void AWildOmissionPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWildOmissionPlayerController, BedUniqueID, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWildOmissionPlayerController, SpawnChunk, COND_OwnerOnly);
}

FPlayerSaveData AWildOmissionPlayerController::SavePlayer()
{
	FPlayerSaveData PlayerSaveData;

	if (!HasAuthority())
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
	PlayerSaveData.BedWorldLocation = BedWorldLocation;
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

	// TODO this is a no no
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
	BedWorldLocation = SaveData.BedWorldLocation;

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

void AWildOmissionPlayerController::SetCurrentBed(const int32& InBedUniqueID, const FVector& InBedWorldLocation)
{
	BedUniqueID = InBedUniqueID;
	BedWorldLocation = InBedWorldLocation;
}

int32 AWildOmissionPlayerController::GetBedUniqueID() const
{
	return BedUniqueID;
}

FVector AWildOmissionPlayerController::GetBedWorldLocation() const
{
	return BedWorldLocation;
}

void AWildOmissionPlayerController::Save()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	Server_AddToPendingSaves();

	if (!HasAuthority())
	{
		return;
	}
	
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(World->GetAuthGameMode());
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

void AWildOmissionPlayerController::Server_AddToPendingSaves_Implementation()
{
	ASaveManager* SaveManager = ASaveManager::GetSaveManager();
	if (SaveManager == nullptr)
	{
		return;
	}

	UPlayerSaveManagerComponent* PlayerSaveManager = SaveManager->GetPlayerManager();
	if (PlayerSaveManager == nullptr)
	{
		return;
	}

	PlayerSaveManager->AddToPending(this);
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
		SetupPlayerOnServer();
	}

	if (!IsLocalController())
	{
		return;
	}
	
	StartLoading();

	SetupMusicPlayerComponent();

	HasAuthority() ? SetupLocalAsHost() : SetupLocalAsClient();
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

	UVitalsComponent* CharacterVitalsComponent = WildOmissionCharacter->GetVitalsComponent();
	if (CharacterVitalsComponent == nullptr)
	{
		return;
	}

	CharacterVitalsComponent->SetHealth(StoredPlayerSaveData.Vitals.Health);
	CharacterVitalsComponent->SetHunger(StoredPlayerSaveData.Vitals.Hunger);
	CharacterVitalsComponent->SetThirst(StoredPlayerSaveData.Vitals.Thirst);

	UInventoryComponent* CharacterInventoryComponent = WildOmissionCharacter->GetInventoryComponent();
	UInventoryManipulatorComponent* CharacterInventoryManipulatorComponent = WildOmissionCharacter->GetInventoryManipulatorComponent();
	if (CharacterInventoryComponent == nullptr || CharacterInventoryManipulatorComponent == nullptr)
	{
		return;
	}

	CharacterInventoryComponent->Load(StoredPlayerSaveData.Inventory.ByteData);
	CharacterInventoryManipulatorComponent->LoadSelectedItemFromByteDataAndDropInWorld(StoredPlayerSaveData.SelectedItemByteData);

	StoredPlayerSaveData = FPlayerSaveData();
}

void AWildOmissionPlayerController::SetupPlayerOnServer()
{
	UWorld* World = GetWorld();
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (World == nullptr || ChunkManager == nullptr)
	{
		return;
	}

	const bool UseDefaultSpawn = StoredPlayerSaveData.IsAlive == false || StoredPlayerSaveData.NewPlayer == true;
	const FVector SpawnPoint = UseDefaultSpawn ? ChunkManager->GetWorldSpawnPoint() : StoredPlayerSaveData.WorldLocation;

	// TODO custom spectator class with chunk invoker
	AWildOmissionSpectatorPawn* SpecPawn = World->SpawnActor<AWildOmissionSpectatorPawn>(AWildOmissionSpectatorPawn::StaticClass(), SpawnPoint, FRotator::ZeroRotator);
	if (SpecPawn)
	{
		this->Possess(SpecPawn);
	}

	const float ChunkSize = AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();
	SpawnChunk = FIntVector2(
		FMath::RoundToInt32(SpawnPoint.X / ChunkSize),
		FMath::RoundToInt32(SpawnPoint.Y / ChunkSize));
}

void AWildOmissionPlayerController::StartLoading()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	UWildOmissionGameInstance* GameInstance = UWildOmissionGameInstance::GetWildOmissionGameInstance(World);
	if (GameInstance == nullptr)
	{
		return;
	}

	GameInstance->StartLoading();
	GameInstance->SetLoadingSubtitle(TEXT("Loading world state."));
	UE_LOG(LogPlayerController, Verbose, TEXT("BeginPlay: Brought up loading screen."));
}

void AWildOmissionPlayerController::SetupMusicPlayerComponent()
{
	if (MusicPlayerComponent != nullptr)
	{
		return;
	}

	MusicPlayerComponent = NewObject<UMusicPlayerComponent>(this, UMusicPlayerComponent::StaticClass(), TEXT("MusicPlayerComponent"));
	if (MusicPlayerComponent == nullptr)
	{
		return;
	}
	
	MusicPlayerComponent->RegisterComponent();
}

void AWildOmissionPlayerController::SetupLocalAsHost()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerHandle LoadTimerHandle;
	FTimerDelegate LoadTimerDelegate;
	LoadTimerDelegate.BindUObject(this, &AWildOmissionPlayerController::StopLoading);
	World->GetTimerManager().SetTimer(LoadTimerHandle, LoadTimerDelegate, 2.0f, false);
	UE_LOG(LogPlayerController, Verbose, TEXT("BeginPlay: Setup load timer."));
}

void AWildOmissionPlayerController::SetupLocalAsClient()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerDelegate CheckSpawnChunkValidTimerDelegate;
	CheckSpawnChunkValidTimerDelegate.BindUObject(this, &AWildOmissionPlayerController::CheckSpawnChunkValid);
	World->GetTimerManager().SetTimer(CheckSpawnChunkValidTimerHandle, CheckSpawnChunkValidTimerDelegate, 2.0f, true);
	UE_LOG(LogPlayerController, Verbose, TEXT("BeginPlay: Setup check spawn chunk timer."));
}

void AWildOmissionPlayerController::CheckSpawnChunkValid()
{
	UWorld* World = GetWorld();
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (World == nullptr || ChunkManager == nullptr)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("CheckSpawnChunkValid: Failed because World or ChunkManager is nullptr."))
		return;
	}

	FSpawnedChunk SpawnedChunk;
	ChunkManager->GetSpawnedChunk(SpawnChunk, SpawnedChunk);

	const int32 DataCount = (AChunk::GetVertexSize() + 1) * (AChunk::GetVertexSize() + 1);
	if (!IsValid(SpawnedChunk.Chunk) || SpawnedChunk.Chunk->GetHeightData().Num() != DataCount)
	{
		UE_LOG(LogPlayerController, Verbose, TEXT("CheckSpawnChunkValid: Chunk is still loading."));
		return;
	}

	UE_LOG(LogPlayerController, Verbose, TEXT("CheckSpawnChunkValid: Chunk loaded successfully, requesting spawn."));
	World->GetTimerManager().ClearTimer(CheckSpawnChunkValidTimerHandle);
	StopLoading();
}

void AWildOmissionPlayerController::StopLoading()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	Server_Spawn();
	
	UWildOmissionGameInstance* GameInstance = UWildOmissionGameInstance::GetWildOmissionGameInstance(World);
	if (GameInstance == nullptr)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Couldnt Stop Loading, GameInstance returned a nullptr."));
		return;
	}

	GameInstance->StopLoading();
}

void AWildOmissionPlayerController::Server_SendMessage_Implementation(APlayerState* Sender, const FString& Message)
{
	AGameChatManager* ChatManager = AGameChatManager::GetGameChatManager();
	if (ChatManager == nullptr)
	{
		return;
	}

	ChatManager->SendMessage(Sender, Message, false);
}

void AWildOmissionPlayerController::Server_Spawn_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(World->GetAuthGameMode());
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
		bIsStillLoading = false;
	}
}

void AWildOmissionPlayerController::Client_ShowDeathMenu_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr || !IsLocalController())
	{
		return;
	}

	// Make Sure to Close Pause Menu So it Doesn't Get Stuck on Screen!
	UWildOmissionGameInstance* WOGameInstance = UWildOmissionGameInstance::GetWildOmissionGameInstance(World);
	if (WOGameInstance)
	{
		UGameplayMenuWidget* OpenGameplayMenu = WOGameInstance->GetGameplayMenuWidget();
		if (OpenGameplayMenu)
		{
			OpenGameplayMenu->Teardown();
		}
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