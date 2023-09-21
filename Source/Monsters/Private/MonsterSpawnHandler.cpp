// Copyright Telephone Studios. All Rights Reserved.


#include "MonsterSpawnHandler.h"
#include "TimeOfDayHandler.h"
#include "Engine/DataTable.h"
#include "Monsters/Monster.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMonsterSpawnHandler::AMonsterSpawnHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MinSpawnCheckTimeSeconds = 5.0f;
	MaxSpawnCheckTimeSeconds = 15.0f;
	InnerSpawnRadiusCentimeters = 5000.0f;
	OuterSpawnRadiusCentimeters = 10000.0f;
	SpawnChance = 1.0f;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterSpawnDataTableObject(TEXT("/Game/Monsters/DataTables/DT_MonsterSpawnData"));
	if (MonsterSpawnDataTableObject.Succeeded())
	{
		AISpawnDataTable = MonsterSpawnDataTableObject.Object;
	}
}

void AMonsterSpawnHandler::Setup(ATimeOfDayHandler* InTimeOfDayHandler)
{
	TimeOfDayHandler = InTimeOfDayHandler;
	OnRep_TimeOfDayHandler();
}

void AMonsterSpawnHandler::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMonsterSpawnHandler, TimeOfDayHandler, COND_InitialOnly);
}

// Called when the game starts or when spawned
void AMonsterSpawnHandler::BeginPlay()
{
	Super::BeginPlay();

}

bool AMonsterSpawnHandler::IsSpawnConditionValid()
{
	if (TimeOfDayHandler && TimeOfDayHandler->IsNight())
	{
		return true;
	}

	return false;
}

void AMonsterSpawnHandler::OnRep_TimeOfDayHandler()
{
	AMonster::SetTimeOfDayHandler(TimeOfDayHandler);
}
