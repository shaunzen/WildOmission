// Copyright Telephone Studios. All Rights Reserved.


#include "ActorSaveHandlerComponent.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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
		if (IsValid(Actor) || !Actor->Implements<USavableObjectInterface>())
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
		
		OutSaves.Add(ActorData);
	}
}

void UActorSaveHandlerComponent::LoadActors(const TArray<FActorSaveData>& InSaves)
{
	for (const FActorSaveData& ActorData : InSaves)
	{
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
	}
}