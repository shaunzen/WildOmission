// Copyright Telephone Studios. All Rights Reserved.


#include "ActorSaveHandlerComponent.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UActorSaveHandlerComponent::UActorSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UActorSaveHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UActorSaveHandlerComponent::SaveActors(TArray<FActorSaveData>& OutSaves)
{
	OutSaves.Empty();

	for (FActorIterator Iterator(GetWorld()); Iterator; ++Iterator)
	{
		AActor* Actor = *Iterator;
		if (!IsValid(Actor) || !Actor->Implements<USavableObjectInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.Name = Actor->GetFName();
		ActorData.Transform = Actor->GetActorTransform();
		ActorData.Class = Actor->GetClass();

		FMemoryWriter MemoryWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);

		for (UActorComponent* ActorComponent : Actor->GetComponentsByInterface(USavableObjectInterface::StaticClass()))
		{
			FActorComponentSaveData ComponentSaveData;
			ComponentSaveData.Name = ActorComponent->GetFName();
			UE_LOG(LogTemp, Warning, TEXT("Comp name %s"), *ActorComponent->GetFName().ToString());

			ComponentSaveData.Class = ActorComponent->GetClass();
			
			FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryWriter, true);
			ComponentArchive.ArIsSaveGame = true;
			ActorComponent->Serialize(ComponentArchive);

			ActorData.ComponentData.Add(ComponentSaveData);
		}

		OutSaves.Add(ActorData);
	}
}

void UActorSaveHandlerComponent::LoadActors(const TArray<FActorSaveData>& InSaves)
{
	for (const FActorSaveData& ActorData : InSaves)
	{
		if (ActorData.Class == nullptr)
		{
			continue;
		}

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorData.Class, ActorData.Transform);
		if (SpawnedActor == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load actor from save file: %s"), *ActorData.Name.ToString());
			return;
		}
		
		FMemoryReader MemoryReader(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;

		SpawnedActor->Serialize(Archive);

		for (UActorComponent* ActorComponent : SpawnedActor->GetComponentsByInterface(USavableObjectInterface::StaticClass()))
		{
			FActorComponentSaveData ComponentSave = FindComponentDataByName(ActorData.ComponentData, ActorComponent->GetFName(), ActorComponent->GetClass());
			FMemoryReader ComponentMemoryReader(ComponentSave.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryReader, true);
			ComponentArchive.ArIsSaveGame = true;

			ActorComponent->Serialize(ComponentArchive);
		}

		ISavableObjectInterface::Execute_OnActorLoaded(SpawnedActor);
	}
}

TArray<FActorComponentSaveData> UActorSaveHandlerComponent::FindComponentsByClass(const TArray<FActorComponentSaveData>& ComponentSaveList, UClass* ClassToFind)
{
	TArray<FActorComponentSaveData> SortedList;

	for (const FActorComponentSaveData& ComponentData : ComponentSaveList)
	{
		if (ComponentData.Class != ClassToFind)
		{
			continue;
		}
		SortedList.Add(ComponentData);
	}

	return SortedList;
}

FActorComponentSaveData UActorSaveHandlerComponent::FindComponentDataByName(const TArray<FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass)
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