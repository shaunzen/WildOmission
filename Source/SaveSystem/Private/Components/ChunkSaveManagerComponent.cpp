// Copyright Telephone Studios. All Rights Reserved.

#include "Components/ChunkSaveManagerComponent.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Structs/ChunkSaveData.h"
#include "Interfaces/SavableObject.h"
#include "Structs/SavableObjectDefinition.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/DataTable.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static UDataTable* DT_SavableObjectDefinitions = nullptr;

UChunkSaveManagerComponent::UChunkSaveManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> SavableObjectDefinitionsBlueprint(TEXT("/Game/SaveSystem/DataTables/DT_SavableObjectDefinitions"));
	if (SavableObjectDefinitionsBlueprint.Succeeded())
	{
		DT_SavableObjectDefinitions = SavableObjectDefinitionsBlueprint.Object;
	}
}

void UChunkSaveManagerComponent::Generate()
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	ChunkManager->OnWorldGenerationComplete.AddDynamic(this, &UChunkSaveManagerComponent::BroadcastGenerationCompleted);
	ChunkManager->Generate();
}

void UChunkSaveManagerComponent::Save(TArray<FChunkSaveData>& OutData)
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	OutData.Empty();

	TArray<AChunk*> Chunks = ChunkManager->GetChunks();
	for (AChunk* Chunk : Chunks)
	{
		if (!IsValid(Chunk))
		{
			continue;
		}

		FChunkSaveData ChunkSaveData;
		ChunkSaveData.GridLocation = Chunk->GetChunkLocation();

		FMemoryWriter MemoryWriter(ChunkSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Chunk->Serialize(Archive);

		// TODO save all attached actors!

		OutData.Add(ChunkSaveData);
	}
}

void UChunkSaveManagerComponent::Load(const TArray<FChunkSaveData>& InData)
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	ChunkManager->ClearChunks();

	for (const FChunkSaveData& ChunkSaveData : InData)
	{
		const FVector ChunkLocation(ChunkSaveData.GridLocation.X * 1600.0f, ChunkSaveData.GridLocation.Y * 1600.0f, 0.0f);
		AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkManager->GetChunkClass(), ChunkLocation, FRotator::ZeroRotator);

		FMemoryReader MemoryReader(ChunkSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;

		SpawnedChunk->Serialize(Archive);

		// TODO Spawn and load all attached actors!

		SpawnedChunk->OnLoadFromSaveComplete();
		ChunkManager->AddChunk(SpawnedChunk);
	}
}

void UChunkSaveManagerComponent::BroadcastGenerationCompleted()
{
	if (!OnWorldGenerationCompleted.IsBound())
	{
		return;
	}

	OnWorldGenerationCompleted.Broadcast();
}
