// Copyright Telephone Studios. All Rights Reserved.


#include "AnimalSpawnManager.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

// Sets default values
AAnimalSpawnManager::AAnimalSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MinSpawnCheckTimeSeconds = 5.0f;
	MaxSpawnCheckTimeSeconds = 15.0f;
	InnerSpawnRadiusCentimeters = 3000.0f;
	OuterSpawnRadiusCentimeters = 4000.0f;
	SpawnChance = 0.1f;

	static ConstructorHelpers::FObjectFinder<UDataTable> AnimalSpawnDataTableObject(TEXT("/Game/Animals/DataTables/DT_AnimalSpawnData"));
	if (AnimalSpawnDataTableObject.Succeeded())
	{
		AISpawnDataTable = AnimalSpawnDataTableObject.Object;
	}
}

// Called when the game starts or when spawned
void AAnimalSpawnManager::BeginPlay()
{
	Super::BeginPlay();
}