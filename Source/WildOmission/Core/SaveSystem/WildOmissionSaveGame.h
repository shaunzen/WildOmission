// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WildOmission/Core/Structs/InventorySlot.h"
#include "WildOmissionSaveGame.generated.h"

USTRUCT()
struct FWildOmissionInventorySave
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FInventoryItem> Items;
	
	UPROPERTY()
	TArray<FInventorySlot> Slots;

};

USTRUCT()
struct FVitalsSave
{
	GENERATED_BODY()

	UPROPERTY()
	float Health = 50.0f;
	
	UPROPERTY()
	float Hunger = 100.0f;
	
	UPROPERTY()
	float Thirst = 100.0f;
};

USTRUCT()
struct FWildOmissionPlayerSave
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool IsHost = false;

	UPROPERTY()
	bool IsAlive = false;

	UPROPERTY()
	FString UniqueID = FString("");
	
	UPROPERTY()
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY()
	FVitalsSave Vitals;

	UPROPERTY()
	FWildOmissionInventorySave Inventory;

	UPROPERTY()
	FInventoryItem SelectedItem;

};

USTRUCT()
struct FActorComponentSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	UClass* Class = nullptr;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName Name = FName();
	
	UPROPERTY()
	FTransform Transform = FTransform::Identity;
	
	UPROPERTY()
	UClass* Class = nullptr;

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TArray<FActorComponentSaveData> ComponentData;

};

USTRUCT()
struct FWeatherManagerSave
{
	GENERATED_BODY()
	
	UPROPERTY()
	float NextStormSpawnTime = 0.0f;

};

USTRUCT()
struct FWildOmissionSaveCreationInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name = FString();

	UPROPERTY()
	bool LevelHasGenerated = false;

	UPROPERTY()
	uint8 Day = 1;
	
	UPROPERTY()
	uint8 Month = 1;
	
	UPROPERTY()
	uint16 Year = 2023;

};

UCLASS()
class WILDOMISSION_API UWildOmissionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWildOmissionSaveGame();

	UPROPERTY()
	uint32 DaysPlayed = 0;
	
	UPROPERTY()
	FDateTime LastPlayedTime;

	UPROPERTY()
	FWeatherManagerSave WeatherManagerSave;

	UPROPERTY()
	FWildOmissionSaveCreationInformation CreationInformation;

	UPROPERTY()
	TArray<FActorSaveData> ActorSaves;
	
	UPROPERTY()
	TArray<FWildOmissionPlayerSave> PlayerSaves;
};
