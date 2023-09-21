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
	
	void Setup(ATimeOfDayHandler* InTimeOfDayHandler);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool IsSpawnConditionValid() override;

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_TimeOfDayHandler)
	ATimeOfDayHandler* TimeOfDayHandler;

	UFUNCTION()
	void OnRep_TimeOfDayHandler();

};
