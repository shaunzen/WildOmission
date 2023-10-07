// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AISpawnHandler.h"
#include "MonsterSpawnHandler.generated.h"

class ATimeOfDayHandler;

UCLASS()
class MONSTERS_API AMonsterSpawnHandler : public AAISpawnHandler
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnHandler();
	
	static AMonsterSpawnHandler* GetMonsterSpawnHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool IsSpawnConditionValid() override;

};
