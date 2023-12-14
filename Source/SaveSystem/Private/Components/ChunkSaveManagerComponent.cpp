// Copyright Telephone Studios. All Rights Reserved.

#include "Components/ChunkSaveManagerComponent.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Structs/ChunkData.h"
#include "WildOmissionSaveGame.h"
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

// this will only be called when chunk manager requests a save of a specific chunk
void UChunkSaveManagerComponent::Save(TArray<FChunkData>& OutData)
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	OutData.Empty();

	//TSet<AChunk*> Chunks = ChunkManager->GetChunks();
	//for (AChunk* Chunk : Chunks)
	//{
	//	if (!IsValid(Chunk))
	//	{
	//		continue;
	//	}

	//	FChunkData ChunkSaveData;
	//	ChunkSaveData.GridLocation = Chunk->GetChunkLocation();

	//	FMemoryWriter ChunkMemoryWriter(ChunkSaveData.ByteData);
	//	FObjectAndNameAsStringProxyArchive ChunkArchive(ChunkMemoryWriter, true);
	//	ChunkArchive.ArIsSaveGame = true;
	//	Chunk->Serialize(ChunkArchive);

	//	TArray<AActor*> AttachedActors;
	//	Chunk->GetAttachedActors(AttachedActors);

	//	for (AActor* Actor : AttachedActors)
	//	{
	//		if (!IsValid(Actor) || !Actor->Implements<USavableObject>())
	//		{
	//			continue;
	//		}

	//		ISavableObject* SavableObjectActor = Cast<ISavableObject>(Actor);
	//		if (SavableObjectActor == nullptr)
	//		{
	//			UE_LOG(LogSaveSystem, Warning, TEXT("Cannot Cast to SavableObject, Actor: %s"), *Actor->GetActorNameOrLabel());
	//			continue;
	//		}

	//		FActorSaveData ActorData;
	//		ActorData.Identifier = SavableObjectActor->GetIdentifier();
	//		ActorData.Transform = Actor->GetActorTransform();

	//		FMemoryWriter ActorMemoryWriter(ActorData.ByteData);
	//		FObjectAndNameAsStringProxyArchive ActorArchive(ActorMemoryWriter, true);
	//		ActorArchive.ArIsSaveGame = true;
	//		Actor->Serialize(ActorArchive);

	//		TArray<UActorComponent*> SavableComponents = Actor->GetComponentsByInterface(USavableObject::StaticClass());
	//		for (UActorComponent* ActorComponent : SavableComponents)
	//		{
	//			FActorComponentSaveData ComponentSaveData;
	//			ComponentSaveData.Name = ActorComponent->GetFName();
	//			ComponentSaveData.Class = ActorComponent->GetClass();

	//			FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
	//			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryWriter, true);
	//			ComponentArchive.ArIsSaveGame = true;
	//			ActorComponent->Serialize(ComponentArchive);

	//			ActorData.ComponentData.Add(ComponentSaveData);
	//		}

	//		ChunkSaveData.ActorData.Add(ActorData);
	//	}

	//	OutData.Add(ChunkSaveData);
	//}
}

void UChunkSaveManagerComponent::Load(const TArray<FChunkData>& InData, const int32& SaveFileVersion)
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	ChunkManager->ClearChunks();

	for (const FChunkData& ChunkSaveData : InData)
	{
		const FVector ChunkLocation(ChunkSaveData.GridLocation.X * 1600.0f, ChunkSaveData.GridLocation.Y * 1600.0f, 0.0f);
		AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkManager->GetChunkClass(), ChunkLocation, FRotator::ZeroRotator);

		FMemoryReader ChunkMemoryReader(ChunkSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ChunkArchive(ChunkMemoryReader, true);
		ChunkArchive.ArIsSaveGame = true;

		SpawnedChunk->Serialize(ChunkArchive);

		for (const FActorSaveData& ActorSaveData : ChunkSaveData.ActorData)
		{
			UClass* ActorClass = FindSavableObjectClassUsingIdentifier(ActorSaveData.Identifier);

			if (ActorClass == nullptr)
			{
				UE_LOG(LogSaveSystem, Warning, TEXT("Savable Object Definition was unable to find class for %s"), *ActorSaveData.Identifier.ToString());
				continue;
			}

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, ActorSaveData.Transform);
			if (SpawnedActor == nullptr)
			{
				UE_LOG(LogSaveSystem, Warning, TEXT("Failed to load actor from save file: %s"), *ActorSaveData.Identifier.ToString());
				continue;
			}

			SpawnedActor->AttachToActor(SpawnedChunk, FAttachmentTransformRules::KeepWorldTransform);

			FMemoryReader ActorMemoryReader(ActorSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ActorArchive(ActorMemoryReader, true);
			ActorArchive.ArIsSaveGame = true;

			SpawnedActor->Serialize(ActorArchive);

			TArray<UActorComponent*> SavableComponents = SpawnedActor->GetComponentsByInterface(USavableObject::StaticClass());
			for (UActorComponent* ActorComponent : SavableComponents)
			{
				FActorComponentSaveData ComponentSave = FindComponentDataByName(ActorSaveData.ComponentData, ActorComponent->GetFName(), ActorComponent->GetClass());
				FMemoryReader ComponentMemoryReader(ComponentSave.ByteData);
				FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryReader, true);
				ComponentArchive.ArIsSaveGame = true;

				ActorComponent->Serialize(ComponentArchive);
				ISavableObject::Execute_OnLoadComplete(ActorComponent);
			}

			if (SaveFileVersion < CURRENT_SAVE_FILE_VERSION)
			{
				FixSaveCompatibility(SpawnedActor, SaveFileVersion);
			}

			ISavableObject::Execute_OnLoadComplete(SpawnedActor);
		}
		
		SpawnedChunk->OnLoadFromSaveComplete();
		ChunkManager->AddChunk(SpawnedChunk);
	}
}

FActorComponentSaveData UChunkSaveManagerComponent::FindComponentDataByName(const TArray<FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass)
{
	for (const FActorComponentSaveData& ComponentData : ComponentSaveList)
	{
		if (ComponentData.Name != ComponentName || (ComponentClass != nullptr && ComponentData.Class != ComponentClass))
		{
			continue;
		}

		return ComponentData;
	}

	return FActorComponentSaveData();
}

UClass* UChunkSaveManagerComponent::FindSavableObjectClassUsingIdentifier(const FName& Identifier)
{
	if (DT_SavableObjectDefinitions == nullptr)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("SavableObjectDefinitions DataTable is nullptr."));
		return nullptr;
	}

	static const FString ContextString = TEXT("SavableObjectDefinition Context");

	FSavableObjectDefinition* ObjectDefinition = DT_SavableObjectDefinitions->FindRow<FSavableObjectDefinition>(Identifier, ContextString, true);
	if (ObjectDefinition == nullptr)
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("Couldn't find SavableObjectDefinition for %s."), *Identifier.ToString());
		return nullptr;
	}

	return ObjectDefinition->Class.Get();
}

void UChunkSaveManagerComponent::FixSaveCompatibility(AActor* ActorToFix, const int32& OldSaveFileVersion)
{
	// Sea Level Change
	if (OldSaveFileVersion <= 0)
	{
		const FVector OldActorLocation = ActorToFix->GetActorLocation();
		const FVector SeaLevelDifference = FVector(0.0f, 0.0f, 360.0f);
		ActorToFix->SetActorLocation(OldActorLocation - SeaLevelDifference);
	}
	// for future fuckups use if (OldSaveFileVersion <= 1, 2, 3, 4, 5, etc)
}



void UChunkSaveManagerComponent::BroadcastGenerationCompleted()
{
	if (!OnWorldGenerationCompleted.IsBound())
	{
		return;
	}

	OnWorldGenerationCompleted.Broadcast();
}
