// Copyright Telephone Studios. All Rights Reserved.


#include "Components/WeatherSaveComponent.h"
#include "WeatherManager.h"
#include "Actors/Storm.h"
#include "Structs/WeatherData.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Log.h"

// Sets default values for this component's properties
UWeatherSaveComponent::UWeatherSaveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UWeatherSaveComponent::Save(FWeatherData& OutWeatherData)
{
	UE_LOG(LogWeather, Verbose, TEXT("Begin saving weather."));

	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (!IsValid(WeatherManager))
	{
		UE_LOG(LogWeather, Warning, TEXT("No weather manager, save failed."));
		return;
	}

	OutWeatherData.StormData.Reset();
	
	AStorm* CurrentStorm = WeatherManager->GetCurrentStorm();
	if (!IsValid(CurrentStorm))
	{
		UE_LOG(LogWeather, Verbose, TEXT("No storm, saved successfully."));
		return;
	}
	
	FStormData StormData;

	FMemoryWriter MemoryWriter(StormData.ByteData);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	CurrentStorm->Serialize(Archive);

	OutWeatherData.StormData.Add(StormData);

	UE_LOG(LogWeather, Verbose, TEXT("Saving weather complete."));
}

void UWeatherSaveComponent::Load(const FWeatherData& InWeatherData)
{
	UE_LOG(LogWeather, Verbose, TEXT("Begin weather load."));
	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (!IsValid(WeatherManager))
	{
		UE_LOG(LogWeather, Warning, TEXT("Weather load failed, no weather manager."));
		return;
	}

	if (InWeatherData.StormData.IsEmpty())
	{
		UE_LOG(LogWeather, Verbose, TEXT("Weather load successfull, no storms found in save."));
		return;
	}

	AStorm* CurrentStorm = WeatherManager->SpawnStorm();
	if (!IsValid(CurrentStorm))
	{
		UE_LOG(LogWeather, Warning, TEXT("Weather load failed, storm failed to spawn."));
		return;
	}


	FMemoryReader MemoryReader(InWeatherData.StormData[0].ByteData);
	FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
	Archive.ArIsSaveGame = true;
	CurrentStorm->Serialize(Archive);

	CurrentStorm->OnLoadComplete();

	UE_LOG(LogWeather, Verbose, TEXT("Finished weather load."));
}