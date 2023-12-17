// Copyright Telephone Studios. All Rights Reserved.

#include "Components/ChunkSaveManagerComponent.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Structs/ChunkData.h"
#include "WildOmissionSaveGame.h"
//#include "Structs/SavableObjectDefinition.h"
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

void UChunkSaveManagerComponent::Generate(const uint32& Seed)
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	
	// TODO set seed

	BroadcastGenerationCompleted();
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
	OutData = ChunkManager->GetChunksData();
}

void UChunkSaveManagerComponent::Load(const TArray<FChunkData>& InData,const uint32& Seed, const int32& SaveFileVersion)
{
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}
	ChunkManager->SetGenerationSeed(Seed);
	ChunkManager->SetChunkData(InData);
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

	//FSavableObjectDefinition* ObjectDefinition = DT_SavableObjectDefinitions->FindRow<FSavableObjectDefinition>(Identifier, ContextString, true);
	/*if (ObjectDefinition == nullptr)
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("Couldn't find SavableObjectDefinition for %s."), *Identifier.ToString());
		return nullptr;
	}*/

	//return ObjectDefinition->Class.Get();
	return nullptr;
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
