// Copyright Telephone Studios. All Rights Reserved.


#include "Components/WeatherSaveComponent.h"
#include "WeatherManager.h"
#include "Actors/Storm.h"
#include "Structs/WeatherData.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Saving Weather."));

	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (!IsValid(WeatherManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Weather Manager, Save Failed."));
		return;
	}

	OutWeatherData.StormData.Reset();
	
	AStorm* CurrentStorm = WeatherManager->GetCurrentStorm();
	if (!IsValid(CurrentStorm))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Storm, Saved Successfully."));
		return;
	}
	
	FStormData StormData;

	FMemoryWriter MemoryWriter(StormData.ByteData);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	CurrentStorm->Serialize(Archive);

	OutWeatherData.StormData.Add(StormData);

	UE_LOG(LogTemp, Warning, TEXT("Saving Weather Complete."));
}

void UWeatherSaveComponent::Load(const FWeatherData& InWeatherData)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Weather Load."));
	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (!IsValid(WeatherManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weather Load Failed, No Weather Manager."));
		return;
	}

	if (InWeatherData.StormData.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Weather Load Successfull No Storms Found In Save."));
		return;
	}

	AStorm* CurrentStorm = WeatherManager->SpawnStorm();
	if (!IsValid(CurrentStorm))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weather Load Failed, Storm Failed to spawn."));
		return;
	}


	FMemoryReader MemoryReader(InWeatherData.StormData[0].ByteData);
	FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
	Archive.ArIsSaveGame = true;
	CurrentStorm->Serialize(Archive);

	CurrentStorm->OnLoadComplete();

	UE_LOG(LogTemp, Warning, TEXT("Finished Weather Load."));
}