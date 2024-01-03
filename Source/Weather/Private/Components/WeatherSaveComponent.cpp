// Copyright Telephone Studios. All Rights Reserved.


#include "Components/WeatherSaveComponent.h"
#include "WeatherManager.h"
#include "Actors/Storm.h"
#include "Structs/WeatherSave.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values for this component's properties
UWeatherSaveComponent::UWeatherSaveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UWeatherSaveComponent::Save(FWeatherData& OutWeatherData)
{
	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (!IsValid(WeatherManager))
	{
		return;
	}

	OutWeatherData.StormData.Reset();
	
	AStorm* CurrentStorm = WeatherManager->GetCurrentStorm();
	if (!IsValid(CurrentStorm))
	{
		return;
	}
	
	FStormData StormData;

	FMemoryWriter MemoryWriter(StormData.ByteData);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	CurrentStorm->Serialize(Archive);

	OutWeatherData.StormData.Add(StormData);

}

void UWeatherSaveComponent::Load(const FWeatherData& InWeatherData)
{
	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (!IsValid(WeatherManager))
	{
		return;
	}

	if (InWeatherData.StormData.IsEmpty())
	{
		return;
	}

	AStorm* CurrentStorm = WeatherManager->SpawnStorm();
	if (!IsValid(CurrentStorm))
	{
		return;
	}


	FMemoryReader MemoryReader(InWeatherData.StormData[0].ByteData);
	FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
	Archive.ArIsSaveGame = true;
	CurrentStorm->Serialize(Archive);

	CurrentStorm->OnLoadComplete();
}