// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AISpawnManager.h"
#include "MonsterSpawnManager.generated.h"

class ATimeOfDayManager;

UCLASS()
class MONSTERS_API AMonsterSpawnManager : public AAISpawnManager
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnManager();
	
	static AMonsterSpawnManager* GetMonsterSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool IsSpawnConditionValid() override;

};
