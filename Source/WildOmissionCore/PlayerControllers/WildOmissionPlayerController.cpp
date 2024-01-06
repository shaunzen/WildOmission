// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Components/VitalsComponent.h"
#include "GameFramework/PlayerState.h"
#include "WildOmissionCore/GameModes/WildOmissionGameMode.h"
#include "SaveManager.h"
#include "GameChatManager.h"
#include "Components/PlayerSaveManagerComponent.h"
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
	SpawnChunk = FIntVector2();

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
	DOREPLIFETIME_CONDITION(AWildOmissionPlayerController, SpawnChunk, COND_OwnerOnly);
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

void AWildOmissionPlayerController::Server_AddToPendingSaves_Implementation()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	GameMode->GetSaveManager()->GetPlayerManager()->AddToPending(this);
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

	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	if (HasAuthority())
	{
		const bool UseDefaultSpawn = StoredPlayerSaveData.IsAlive == false || StoredPlayerSaveData.NewPlayer == true;
		const FVector SpawnPoint = UseDefaultSpawn ? ChunkManager->GetWorldSpawnPoint() : StoredPlayerSaveData.WorldLocation;

		ASpectatorPawn* SpecPawn = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), SpawnPoint, FRotator::ZeroRotator);
		if (SpecPawn)
		{
			this->Possess(SpecPawn);
		}

		const float ChunkSize = AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();
		SpawnChunk = FIntVector2(
			FMath::RoundToInt32(SpawnPoint.X / ChunkSize),
			FMath::RoundToInt32(SpawnPoint.Y / ChunkSize));
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
		GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, LoadTimerDelegate, 2.0f, false);
	}
	else
	{
		FTimerDelegate CheckSpawnChunkValidTimerDelegate;
		CheckSpawnChunkValidTimerDelegate.BindUObject(this, &AWildOmissionPlayerController::CheckSpawnChunkValid);
		GetWorld()->GetTimerManager().SetTimer(CheckSpawnChunkValidTimerHandle, CheckSpawnChunkValidTimerDelegate, 2.0f, true);
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

void AWildOmissionPlayerController::CheckSpawnChunkValid()
{
	UWorld* World = GetWorld();
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (World == nullptr || ChunkManager == nullptr)
	{
		return;
	}

	FSpawnedChunk SpawnedChunk;
	ChunkManager->GetSpawnedChunk(SpawnChunk, SpawnedChunk);

	const int32 DataCount = (AChunk::GetVertexSize() + 1) * (AChunk::GetVertexSize() + 1);
	if (IsValid(SpawnedChunk.Chunk))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnChunkDataCount %i, Required %i"), SpawnedChunk.Chunk->GetHeightData().Num(), DataCount);
	}

	if (!IsValid(SpawnedChunk.Chunk) || SpawnedChunk.Chunk->GetHeightData().Num() != DataCount)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(CheckSpawnChunkValidTimerHandle);
	StopLoading();
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
	AGameChatManager* ChatManager = AGameChatManager::GetGameChatManager();
	if (ChatManager == nullptr)
	{
		return;
	}

	ChatManager->SendMessage(Sender, Message, false);
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

	// TODO why is this here, im confused, this whole loading thing goes all over the place
	if (bIsStillLoading == true)
	{
		OnFinishedLoading.Broadcast(this);
		bIsStillLoading = false;
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