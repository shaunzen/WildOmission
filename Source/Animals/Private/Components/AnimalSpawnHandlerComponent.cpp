// Copyright Telephone Studios. All Rights Reserved.


#include "Components/AnimalSpawnHandlerComponent.h"
#include "Animals/Animal.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

const static int32 MIN_SPAWN_CHECK_TIME_SECONDS = 5.0f;
const static int32 MAX_SPAWN_CHECK_TIME_SECONDS = 15.0f;
static UDataTable* AnimalSpawnDataTable = nullptr;

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

	CheckSpawnConditions();

}

// Called every frame
void UAnimalSpawnHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimalSpawnHandlerComponent::CheckSpawnConditions()
{

	// Check how many animals are in range of this component
	TArray<AActor*> AnimalActorsList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnimal::StaticClass(), AnimalActorsList);
	const float SearchRadiusCentimeters = 10000.0f;
	const int32 AnimalsInRange = GetNumActorsWithinRange(AnimalActorsList, SearchRadiusCentimeters);

	// Set timer to call this function again in the future
	float NextCheckTimeSeconds = FMath::RandRange(MIN_SPAWN_CHECK_TIME_SECONDS, MAX_SPAWN_CHECK_TIME_SECONDS);
	FTimerDelegate NextSpawnCheckTimerDelegate;
	NextSpawnCheckTimerDelegate.BindUObject(this, &UAnimalSpawnHandlerComponent::CheckSpawnConditions);
	GetWorld()->GetTimerManager().SetTimer(NextSpawnCheckTimerHandler, NextSpawnCheckTimerDelegate, NextCheckTimeSeconds, false);

	// If no animals are present, there is a chance we will spawn some
	if (AnimalsInRange != 0 || !UKismetMathLibrary::RandomBoolWithWeight(0.1f))
	{
		return;
	}

	SpawnAnimals();
}

int32 UAnimalSpawnHandlerComponent::GetNumActorsWithinRange(const TArray<AActor*>& Actors, const float& Distance) const
{
	int32 ActorsInRange = 0;

	for (AActor* Actor : Actors)
	{
		if (Actor == nullptr || FVector::Distance(Actor->GetActorLocation(), GetComponentLocation()) > Distance)
		{
			continue;
		}
		++ActorsInRange;
	}

	return ActorsInRange;
}

void UAnimalSpawnHandlerComponent::SpawnAnimals()
{

}
