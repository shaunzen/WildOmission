// Copyright Telephone Studios. All Rights Reserved.


#include "Components/AnimalSpawnHandlerComponent.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

const static int32 MIN_SPAWN_CHECK_TIME_SECONDS = 5.0f;
const static int32 MAX_SPAWN_CHECK_TIME_SECONDS = 15.0f;
static UDataTable* AnimalSpawnDataTable = nullptr;
// Need some kind of data table for referencing the diferent animals to spawn


// Sets default values for this component's properties
UAnimalSpawnHandlerComponent::UAnimalSpawnHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> AnimalDataTableObject(TEXT("/Game/Animals/DataTables/DT_AnimalSpawnData"));
	if (AnimalDataTableObject.Succeeded())
	{
		AnimalSpawnDataTable = AnimalDataTableObject.Object;
	}
}


// Called when the game starts
void UAnimalSpawnHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAnimalSpawnHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

