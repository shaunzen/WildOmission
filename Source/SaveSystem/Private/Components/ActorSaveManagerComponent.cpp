// Copyright Telephone Studios. All Rights Reserved.


#include "Components/ActorSaveManagerComponent.h"
#include "Interfaces/SavableObject.h"
#include "Structs/SavableObjectDefinition.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/DataTable.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

//static UDataTable* DT_SavableObjectDefinitions = nullptr;

// Sets default values for this component's properties
UActorSaveManagerComponent::UActorSaveManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	/*static ConstructorHelpers::FObjectFinder<UDataTable> SavableObjectDefinitionsBlueprint(TEXT("/Game/SaveSystem/DataTables/DT_SavableObjectDefinitions"));
	if (SavableObjectDefinitionsBlueprint.Succeeded())
	{
		DT_SavableObjectDefinitions = SavableObjectDefinitionsBlueprint.Object;
	}*/
}

// Called when the game starts
void UActorSaveManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UActorSaveManagerComponent::SaveActors(TArray<FActorSaveData>& OutSaves)
{
	/*OutSaves.Empty();

	for (FActorIterator Iterator(GetWorld()); Iterator; ++Iterator)
	{
		AActor* Actor = *Iterator;
		if (!IsValid(Actor) || !Actor->Implements<USavableObject>())
		{
			continue;
		}

		ISavableObject* SavableObjectActor = Cast<ISavableObject>(Actor);
		if (SavableObjectActor == nullptr)
		{
			UE_LOG(LogSaveSystem, Warning, TEXT("Cannot Cast to SavableObject, Actor: %s"), *Actor->GetActorNameOrLabel());
			continue;
		}

		FActorSaveData ActorData;
		ActorData.Identifier = SavableObjectActor->GetIdentifier();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);

		TArray<UActorComponent*> SavableComponents = Actor->GetComponentsByInterface(USavableObject::StaticClass());
		for (UActorComponent* ActorComponent : SavableComponents)
		{
			FActorComponentSaveData ComponentSaveData;
			ComponentSaveData.Name = ActorComponent->GetFName();
			ComponentSaveData.Class = ActorComponent->GetClass();
			
			FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryWriter, true);
			ComponentArchive.ArIsSaveGame = true;
			ActorComponent->Serialize(ComponentArchive);

			ActorData.ComponentData.Add(ComponentSaveData);
		}

		OutSaves.Add(ActorData);
	}*/
}

void UActorSaveManagerComponent::LoadActors(const TArray<FActorSaveData>& InSaves, const int32& SaveFileVersion)
{
	/*for (const FActorSaveData& ActorData : InSaves)
	{
		UClass* ActorClass = FindSavableObjectClassUsingIdentifier(ActorData.Identifier);
			
		if (ActorClass == nullptr)
		{
			UE_LOG(LogSaveSystem, Warning, TEXT("Savable Object Definition was unable to find class for %s"), *ActorData.Identifier.ToString());
			continue;
		}

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, ActorData.Transform);
		if (SpawnedActor == nullptr)
		{
			UE_LOG(LogSaveSystem, Warning, TEXT("Failed to load actor from save file: %s"), *ActorData.Identifier.ToString());
			continue;
		}

		FMemoryReader MemoryReader(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;

		SpawnedActor->Serialize(Archive);

		TArray<UActorComponent*> SavableComponents = SpawnedActor->GetComponentsByInterface(USavableObject::StaticClass());
		for (UActorComponent* ActorComponent : SavableComponents)
		{
			FActorComponentSaveData ComponentSave = FindComponentDataByName(ActorData.ComponentData, ActorComponent->GetFName(), ActorComponent->GetClass());
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
	}*/

}

TArray<FActorComponentSaveData> UActorSaveManagerComponent::FindComponentsByClass(const TArray<FActorComponentSaveData>& ComponentSaveList, UClass* ClassToFind)
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

FActorComponentSaveData UActorSaveManagerComponent::FindComponentDataByName(const TArray<FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass)
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

UClass* UActorSaveManagerComponent::FindSavableObjectClassUsingIdentifier(const FName& Identifier)
{
	/*if (DT_SavableObjectDefinitions == nullptr)
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
	}*/

	return nullptr; //ObjectDefinition->Class.Get();
}

void UActorSaveManagerComponent::FixSaveCompatibility(AActor* ActorToFix, const int32& OldSaveFileVersion)
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
