// Copyright Telephone Studios. All Rights Reserved.


#include "Components/ActorSaveHandlerComponent.h"
#include "Interfaces/SavableObject.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/DataTable.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static UDataTable* DT_SavableObjectDefinitions = nullptr;

// Sets default values for this component's properties
UActorSaveHandlerComponent::UActorSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> SavableObjectDefinitionsBlueprint(TEXT("/Game/SaveSystem/DataTables/DT_SavableObjectDefinitions"));
	if (SavableObjectDefinitionsBlueprint.Succeeded())
	{
		DT_SavableObjectDefinitions = SavableObjectDefinitionsBlueprint.Object;
	}
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
		if (!IsValid(Actor) || !Actor->Implements<USavableObject>())
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
	}
}

void UActorSaveHandlerComponent::LoadActors(const TArray<FActorSaveData>& InSaves, const int32& SaveFileVersion)
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
			UE_LOG(LogSaveSystem, Warning, TEXT("Failed to load actor from save file: %s"), *ActorData.Name.ToString());
			return;
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

void UActorSaveHandlerComponent::FixSaveCompatibility(AActor* ActorToFix, const int32& OldSaveFileVersion)
{
	if (OldSaveFileVersion <= 0)
	{
		const FVector OldActorLocation = ActorToFix->GetActorLocation();
		const FVector SeaLevelDifference = FVector(0.0f, 0.0f, 360.0f);
		ActorToFix->SetActorLocation(OldActorLocation - SeaLevelDifference);
	}
	// for future fuckups use if (OldSaveFileVersion <= 1, 2, 3, 4, 5, etc)
}
