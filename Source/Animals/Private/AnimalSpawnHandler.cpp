// Copyright Telephone Studios. All Rights Reserved.


#include "AnimalSpawnHandler.h"
#include "Animals/Animal.h"
#include "Structs/AnimalSpawnData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

const static int32 MIN_SPAWN_CHECK_TIME_SECONDS = 5.0f;
const static int32 MAX_SPAWN_CHECK_TIME_SECONDS = 15.0f;
const static float OUTER_SPAWN_RADIUS_CENTIMETERS = 4000.0f;
const static float INNER_SPAWN_RADIUS_CENTIMETERS = 3000.0f;

static UDataTable* AnimalSpawnDataTable = nullptr;
static TArray<AAnimal*> SpawnedAnimals;

// Sets default values
AAnimalSpawnHandler::AAnimalSpawnHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UDataTable> AnimalDataTableObject(TEXT("/Game/Animals/DataTables/DT_AnimalSpawnData"));
	if (AnimalDataTableObject.Succeeded())
	{
		AnimalSpawnDataTable = AnimalDataTableObject.Object;
	}
}

TArray<AAnimal*>* AAnimalSpawnHandler::GetSpawnedAnimals()
{
	return &SpawnedAnimals;
}

// Called when the game starts or when spawned
void AAnimalSpawnHandler::BeginPlay()
{
	Super::BeginPlay();

	SpawnedAnimals.Empty();

	CheckSpawnConditionsForAllPlayers();
}

void AAnimalSpawnHandler::CheckSpawnConditionsForAllPlayers()
{
	UE_LOG(LogAnimals, Verbose, TEXT("Checking animal spawn conditions."));
	
	// Set timer to call this function again in the future
	const float NextCheckTimeSeconds = FMath::RandRange(MIN_SPAWN_CHECK_TIME_SECONDS, MAX_SPAWN_CHECK_TIME_SECONDS);
	UE_LOG(LogAnimals, VeryVerbose, TEXT("Next condition check set for %f seconds."), NextCheckTimeSeconds);
	FTimerDelegate NextSpawnCheckTimerDelegate;
	NextSpawnCheckTimerDelegate.BindUObject(this, &AAnimalSpawnHandler::CheckSpawnConditionsForAllPlayers);
	GetWorld()->GetTimerManager().SetTimer(NextSpawnCheckTimerHandle, NextSpawnCheckTimerDelegate, NextCheckTimeSeconds, false);

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr || PlayerController->GetPawn() == nullptr)
		{
			continue;
		}

		CheckSpawnConditionsForPlayer(PlayerController->GetPawn());
	}
}

void AAnimalSpawnHandler::CheckSpawnConditionsForPlayer(APawn* Player)
{
	if (Player == nullptr)
	{
		UE_LOG(LogAnimals, Warning, TEXT("Tried to check animal spawn conditions for player, but the player was nullptr."))
		return;
	}

	// Check how many animals are in range of this component
	const int32 AnimalsInRange = GetNumAnimalsWithinRadiusFromLocation(Player->GetActorLocation());
	UE_LOG(LogAnimals, VeryVerbose, TEXT("%i animals found in range of player."), AnimalsInRange);

	// If no animals are present, there is a chance we will spawn some
	if (AnimalsInRange != 0 || !UKismetMathLibrary::RandomBoolWithWeight(0.1f))
	{
		UE_LOG(LogAnimals, Verbose, TEXT("Animal spawn condition not met."));
		return;
	}

	SpawnAnimalsInRadiusFromOrigin(Player->GetActorLocation());
}

int32 AAnimalSpawnHandler::GetNumAnimalsWithinRadiusFromLocation(const FVector& TestLocation) const
{
	if (SpawnedAnimals.IsEmpty())
	{
		return 0;
	}

	int32 AnimalsInRange = 0;

	for (AAnimal* Animal : SpawnedAnimals)
	{
		if (Animal == nullptr || FVector::Distance(Animal->GetActorLocation(), TestLocation) > OUTER_SPAWN_RADIUS_CENTIMETERS)
		{
			continue;
		}
		++AnimalsInRange;
	}

	return AnimalsInRange;
}

void AAnimalSpawnHandler::SpawnAnimalsInRadiusFromOrigin(const FVector& SpawnOrigin)
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
		AAnimal* SpawnedAnimal = GetWorld()->SpawnActor<AAnimal>(SpawnData[AnimalToSpawn]->Class, GetSpawnTransform(SpawnOrigin));
		SpawnedAnimals.Add(SpawnedAnimal);
	}
}

FTransform AAnimalSpawnHandler::GetSpawnTransform(const FVector& SpawnOrigin) const
{
	const float TraceHeight = 50000.0f;
	const float SpawnDistance = FMath::RandRange(INNER_SPAWN_RADIUS_CENTIMETERS, OUTER_SPAWN_RADIUS_CENTIMETERS);
	const float SpawnAngle = FMath::RandRange(0.0f, 360.0f);

	FVector SpawnLocationWithinRadius = FVector::ForwardVector * SpawnDistance;
	SpawnLocationWithinRadius = SpawnLocationWithinRadius.RotateAngleAxis(SpawnAngle, FVector::UpVector);

	FVector Start = SpawnLocationWithinRadius + SpawnOrigin;
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

FAnimalSpawnData* AAnimalSpawnHandler::GetSpawnData(const FName& AnimalName)
{
	if (AnimalSpawnDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString = TEXT("AnimalSpawnData Context");
	return AnimalSpawnDataTable->FindRow<FAnimalSpawnData>(AnimalName, ContextString, true);
}
