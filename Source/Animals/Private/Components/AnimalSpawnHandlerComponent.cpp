// Copyright Telephone Studios. All Rights Reserved.


#include "Components/AnimalSpawnHandlerComponent.h"
#include "Animals/Animal.h"
#include "Structs/AnimalSpawnData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

const static int32 MIN_SPAWN_CHECK_TIME_SECONDS = 5.0f;
const static int32 MAX_SPAWN_CHECK_TIME_SECONDS = 15.0f;
const static float SPAWN_RADIUS_CENTIMETERS = 10000.0f;

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
	UE_LOG(LogAnimals, Verbose, TEXT("Checking animal spawn conditions."));

	// Check how many animals are in range of this component
	TArray<AActor*> AnimalActorsList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnimal::StaticClass(), AnimalActorsList);
	const int32 AnimalsInRange = GetNumActorsWithinRange(AnimalActorsList, SPAWN_RADIUS_CENTIMETERS);
	UE_LOG(LogAnimals, VeryVerbose, TEXT("%i animals found in range of player."), AnimalsInRange);

	// Set timer to call this function again in the future
	float NextCheckTimeSeconds = FMath::RandRange(MIN_SPAWN_CHECK_TIME_SECONDS, MAX_SPAWN_CHECK_TIME_SECONDS);
	UE_LOG(LogAnimals, VeryVerbose, TEXT("Next condition check set for %f seconds."), NextCheckTimeSeconds);
	FTimerDelegate NextSpawnCheckTimerDelegate;
	NextSpawnCheckTimerDelegate.BindUObject(this, &UAnimalSpawnHandlerComponent::CheckSpawnConditions);
	GetWorld()->GetTimerManager().SetTimer(NextSpawnCheckTimerHandler, NextSpawnCheckTimerDelegate, NextCheckTimeSeconds, false);

	// If no animals are present, there is a chance we will spawn some
	if (AnimalsInRange != 0 || !UKismetMathLibrary::RandomBoolWithWeight(0.1f))
	{
		UE_LOG(LogAnimals, Verbose, TEXT("Animal spawn condition not met."));
		return;
	}

	SpawnAnimals();
}

int32 UAnimalSpawnHandlerComponent::GetNumActorsWithinRange(const TArray<AActor*>& Actors, const float& Distance) const
{
	int32 ActorsInRange = 0;

	for (AActor* Actor : Actors)
	{
		if (Actor == nullptr || FVector::Distance(Actor->GetActorLocation(), GetOwner()->GetActorLocation()) > Distance)
		{
			continue;
		}
		++ActorsInRange;
	}

	return ActorsInRange;
}

void UAnimalSpawnHandlerComponent::SpawnAnimals()
{
	if (AnimalSpawnDataTable == nullptr)
	{
		return;
	}
	TArray<FAnimalSpawnData*> SpawnData;
	const FString AnimalSpawnDataContextString = TEXT("AnimalSpawnData Context String");

	AnimalSpawnDataTable->GetAllRows(AnimalSpawnDataContextString, SpawnData);

	int32 AnimalToSpawn = FMath::RandRange(0, SpawnData.Num() - 1);

	UE_LOG(LogAnimals, VeryVerbose, TEXT("Spawning animal with ID %i"), AnimalToSpawn);
	for (int32 i = 0; i < SpawnData[AnimalToSpawn]->SpawnGroupSize; ++i)
	{
		GetWorld()->SpawnActor<AAnimal>(SpawnData[AnimalToSpawn]->Class, GetSpawnTransform());
	}
}

FTransform UAnimalSpawnHandlerComponent::GetSpawnTransform() const
{
	const float TraceHeight = 50000.0f;
	const float SpawnDistance = FMath::RandRange(1000.0f, SPAWN_RADIUS_CENTIMETERS);
	const float SpawnAngle = FMath::RandRange(0.0f, 360.0f);
	
	FVector SpawnLocationWithinRadius = FVector::ForwardVector * SpawnDistance;
	SpawnLocationWithinRadius = SpawnLocationWithinRadius.RotateAngleAxis(SpawnAngle, FVector::UpVector);
	
	FVector Start = SpawnLocationWithinRadius + GetOwner()->GetActorLocation();
	Start.Z = TraceHeight;
	FVector End = Start - FVector(0.0f, 0.0f, TraceHeight);

	FTransform SpawnTransform;

	FHitResult HitResult;
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		return SpawnTransform;
	}

	FRotator SpawnRotation = FRotator::ZeroRotator;
	SpawnRotation.Yaw = FMath::RandRange(0.0f, 360.0f);

	SpawnTransform.SetLocation(HitResult.ImpactPoint);
	SpawnTransform.SetRotation(FQuat(SpawnRotation));

	return SpawnTransform;
}

FAnimalSpawnData* UAnimalSpawnHandlerComponent::GetSpawnData(const FName& AnimalName)
{
	if (AnimalSpawnDataTable == nullptr)
	{
		return nullptr;
	}
	
	static const FString ContextString = TEXT("AnimalSpawnData Context");
	return AnimalSpawnDataTable->FindRow<FAnimalSpawnData>(AnimalName, ContextString, true);
}
