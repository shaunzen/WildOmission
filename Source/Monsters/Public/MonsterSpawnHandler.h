// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnHandler.generated.h"

class AMonster;
class ATimeOfDayHandler;
struct FMonsterSpawnData;

UCLASS()
class MONSTERS_API AMonsterSpawnHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnHandler();
	void Setup(ATimeOfDayHandler* InTimeOfDayHandler);

	static TArray<AMonster*>* GetSpawnedMonsters();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FTimerHandle NextSpawnCheckTimerHandle;

	UFUNCTION()
	void CheckSpawnConditionsForAllPlayers();
	void CheckSpawnConditionsForPlayer(APawn* PlayerToCheck);

	UPROPERTY()
	ATimeOfDayHandler* TimeOfDayHandler;

	int32 GetNumMonstersWithinRadiusFromLocation(const FVector& TestLocation) const;

	UFUNCTION()
	void SpawnMonstersInRadiusFromOrigin(const FVector& SpawnOrigin);

	UFUNCTION()
	void RemoveMonsterFromList(AMonster* MonsterToRemove);

	FTransform GetSpawnTransform(const FVector& SpawnOrigin) const;

	static FMonsterSpawnData* GetSpawnData(const FName& MonsterName);

};
