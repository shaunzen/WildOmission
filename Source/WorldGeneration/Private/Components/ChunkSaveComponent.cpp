// Copyright Telephone Studios. All Rights Reserved.


#include "Components/ChunkSaveComponent.h"
#include "Interfaces/SavableObject.h"
#include "Structs/SavableObjectDefinition.h"
#include "Structs/ChunkData.h"
#include "Actors/Chunk.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static UDataTable* DT_SavableObjectDefinitions = nullptr;

// Sets default values for this component's properties
UChunkSaveComponent::UChunkSaveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	// Even more unreal engine being mean bullshit *facepalm*
	if (GetWorld())
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> SavableObjectDefinitionsBlueprint(TEXT("/Game/SaveSystem/DataTables/DT_SavableObjectDefinitions"));
		if (SavableObjectDefinitionsBlueprint.Succeeded())
		{
			DT_SavableObjectDefinitions = SavableObjectDefinitionsBlueprint.Object;
		}
	}
}

void UChunkSaveComponent::Save(FChunkData& OutChunkData, bool AlsoDestroy)
{
	AChunk* OwnerChunk = GetOwnerChunk();
	if (!IsValid(OwnerChunk))
	{
		return;
	}

	OutChunkData.GridLocation = OwnerChunk->GetChunkLocation();
	OutChunkData.Generated = true;
	OutChunkData.HeightData = OwnerChunk->GetHeightData();
	OutChunkData.SurfaceData = OwnerChunk->GetSurfaceData();

	FMemoryWriter ChunkMemoryWriter(OutChunkData.ByteData);
	FObjectAndNameAsStringProxyArchive ChunkArchive(ChunkMemoryWriter, true);
	ChunkArchive.ArIsSaveGame = true;
	OwnerChunk->Serialize(ChunkArchive);

	TArray<AActor*> AttachedActors;
	OwnerChunk->GetAttachedActors(AttachedActors);
	for (AActor* AttachedActor : AttachedActors)
	{
		if (!IsValid(AttachedActor) || !AttachedActor->Implements<USavableObject>())
		{
			continue;
		}

		ISavableObject* SavableObjectActor = Cast<ISavableObject>(AttachedActor);
		if (SavableObjectActor == nullptr)
		{
			UE_LOG(LogWorldGeneration, Warning, TEXT("Cannot Cast to SavableObject, Actor: %s"), *AttachedActor->GetActorNameOrLabel());
			continue;
		}

		FActorSaveData ActorSaveData;
		ActorSaveData.Identifier = SavableObjectActor->GetIdentifier();
		ActorSaveData.Transform = AttachedActor->GetTransform();

		FMemoryWriter ActorMemoryWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ActorArchive(ActorMemoryWriter, true);
		ActorArchive.ArIsSaveGame = true;
		AttachedActor->Serialize(ActorArchive);

		TArray<UActorComponent*> SavableComponents = AttachedActor->GetComponentsByInterface(USavableObject::StaticClass());
		for (UActorComponent* ActorComponent : SavableComponents)
		{
			if (!IsValid(ActorComponent))
			{
				continue;
			}

			FActorComponentSaveData ComponentSaveData;
			ComponentSaveData.Name = ActorComponent->GetFName();
			ComponentSaveData.Class = ActorComponent->GetClass();

			FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryWriter, true);
			ComponentArchive.ArIsSaveGame = true;

			ActorComponent->Serialize(ComponentArchive);

			ActorSaveData.ComponentData.Add(ComponentSaveData);
		}

		OutChunkData.ActorData.Add(ActorSaveData);

		if (AlsoDestroy == true)
		{
			AttachedActor->Destroy();
		}
	}

	if (AlsoDestroy == true)
	{
		OwnerChunk->Destroy();
	}
}

void UChunkSaveComponent::Load(const FChunkData& InChunkData)
{
	UWorld* World = GetWorld();
	AChunk* OwnerChunk = GetOwnerChunk();
	if (World == nullptr || !IsValid(OwnerChunk))
	{
		return;
	}

	OwnerChunk->SetHeightData(InChunkData.HeightData);
	OwnerChunk->SetSurfaceData(InChunkData.SurfaceData);

	FMemoryReader ChunkMemoryReader(InChunkData.ByteData);
	FObjectAndNameAsStringProxyArchive ChunkArchive(ChunkMemoryReader, true);
	ChunkArchive.ArIsSaveGame = true;
	OwnerChunk->Serialize(ChunkArchive);

	for (const FActorSaveData& ActorSaveData : InChunkData.ActorData)
	{
		UClass* ActorClass = FindSavableObjectClassUsingIdentifier(ActorSaveData.Identifier);
		if (ActorClass == nullptr)
		{
			UE_LOG(LogWorldGeneration, Warning, TEXT("Savable Object Definition was unable to find class for %s"), *ActorSaveData.Identifier.ToString());
			continue;
		}

		AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, ActorSaveData.Transform);
		if (SpawnedActor == nullptr)
		{
			UE_LOG(LogWorldGeneration, Warning, TEXT("Failed to load actor from save file: %s"), *ActorSaveData.Identifier.ToString());
			continue;
		}

		SpawnedActor->AttachToActor(OwnerChunk, FAttachmentTransformRules::KeepWorldTransform);

		FMemoryReader ActorMemoryReader(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ActorArchive(ActorMemoryReader, true);
		ActorArchive.ArIsSaveGame = true;
		SpawnedActor->Serialize(ActorArchive);

		TArray<UActorComponent*> SavableComponents = SpawnedActor->GetComponentsByInterface(USavableObject::StaticClass());
		for (UActorComponent* ActorComponent : SavableComponents)
		{
			if (!IsValid(ActorComponent))
			{
				continue;
			}

			FActorComponentSaveData ComponentSaveData = FindComponentDataByName(ActorSaveData.ComponentData, ActorComponent->GetFName(), ActorComponent->GetClass());
			FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryReader, true);
			ComponentArchive.ArIsSaveGame = true;

			ActorComponent->Serialize(ComponentArchive);
			ISavableObject::Execute_OnLoadComplete(ActorComponent);
		}

		ISavableObject::Execute_OnLoadComplete(SpawnedActor);
	}

	OwnerChunk->OnLoadedTerrainData();
}

UClass* UChunkSaveComponent::FindSavableObjectClassUsingIdentifier(const FName& Identifier)
{
	if (DT_SavableObjectDefinitions == nullptr)
	{
		UE_LOG(LogWorldGeneration, Error, TEXT("SavableObjectDefinitions DataTable is nullptr."));
		return nullptr;
	}
	
	static const FString ContextString = TEXT("SavableObjectDefinition Context");

	FSavableObjectDefinition* ObjectDefinition = DT_SavableObjectDefinitions->FindRow<FSavableObjectDefinition>(Identifier, ContextString, true);
	if (ObjectDefinition == nullptr)
	{
		UE_LOG(LogWorldGeneration, Warning, TEXT("Couldn't find SavableObjectDefinition for %s."), *Identifier.ToString());
		return nullptr;
	}

	return ObjectDefinition->Class.Get();
}

FActorComponentSaveData UChunkSaveComponent::FindComponentDataByName(const TArray<FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass)
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

// Called when the game starts
void UChunkSaveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AChunk* UChunkSaveComponent::GetOwnerChunk() const
{
	return Cast<AChunk>(GetOwner());
}
