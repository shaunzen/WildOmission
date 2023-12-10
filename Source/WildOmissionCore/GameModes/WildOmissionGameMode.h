// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WildOmissionGameMode.generated.h"

class ASaveManager;
class AChunkManager;
class ATimeOfDayManager;
class AWeatherManager;
class AAnimalSpawnManager;
class AMonsterSpawnManager;
class AGameChatManager;
class AWildOmissionCharacter;
class AWildOmissionPlayerController;

UCLASS()
class WILDOMISSIONCORE_API AWildOmissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void SpawnHumanForController(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Exec)
	void SaveGame();

	UFUNCTION(BlueprintCallable, Exec)
	void ResetLocationOfAllConnectedPlayers();

	UFUNCTION(BlueprintCallable, Exec)
	void SetTime(float NormalizedTime);

	UFUNCTION(BlueprintCallable, Exec)
	void Weather(const FString& WeatherToSet);

	ASaveManager* GetSaveManager() const;
	AChunkManager* GetChunkManager() const;
	AWeatherManager* GetWeatherManager() const;

	UFUNCTION(Exec)
	void LogPlayerInventoryComponents();

	UFUNCTION(Exec)
	void LogPlayerInventorySlots();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> HumanCharacterClass;

	bool FriendsOnly = true;

	UPROPERTY()
	ASaveManager* SaveManager;

	UPROPERTY()
	AChunkManager* ChunkManager;
	
	UPROPERTY()
	ATimeOfDayManager* TimeOfDayManager;
	
	UPROPERTY()
	AWeatherManager* WeatherManager;

	UPROPERTY()
	AAnimalSpawnManager* AnimalSpawnManager;

	UPROPERTY()
	AMonsterSpawnManager* MonsterSpawnManager;
	
	UPROPERTY()
	AGameChatManager* ChatManager;

	void SpawnHumanAtStartSpot(AController* Controller);
	void SpawnHumanAtBed(AController* Controller);

};
