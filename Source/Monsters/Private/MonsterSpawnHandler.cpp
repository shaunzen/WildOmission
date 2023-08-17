// Copyright Telephone Studios. All Rights Reserved.


#include "MonsterSpawnHandler.h"
#include "Monsters/Monster.h"
#include "Structs/MonsterSpawnData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

const static int32 MIN_SPAWN_CHECK_TIME_SECONDS = 5.0f;
const static int32 MAX_SPAWN_CHECK_TIME_SECONDS = 15.0f;
const static float OUTER_SPAWN_RADIUS_CENTIMETERS = 4000.0f;
const static float INNER_SPAWN_RADIUS_CENTIMETERS = 3000.0f;

static UDataTable* MonsterSpawnDataTable = nullptr;
static TArray<AMonster*> SpawnedMonsters;

// Sets default values
AMonsterSpawnHandler::AMonsterSpawnHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableObject(TEXT("/Game/Monsters/DataTables/DT_MonsterSpawnData"));
	if (MonsterDataTableObject.Succeeded())
	{
		MonsterSpawnDataTable = MonsterDataTableObject.Object;
	}
}

TArray<AMonster*>* AMonsterSpawnHandler::GetSpawnedMonsters()
{
	return &SpawnedMonsters;
}

// Called when the game starts or when spawned
void AMonsterSpawnHandler::BeginPlay()
{
	Super::BeginPlay();

	SpawnedMonsters.Empty();

	CheckSpawnConditionsForAllPlayers();
}

void AMonsterSpawnHandler::CheckSpawnConditionsForAllPlayers()
{
	UE_LOG(LogMonsters, Verbose, TEXT("Checking monster spawn conditions."));
	
	// Set timer to call this function again in the future
	const float NextCheckTimeSeconds = FMath::RandRange(MIN_SPAWN_CHECK_TIME_SECONDS, MAX_SPAWN_CHECK_TIME_SECONDS);
	UE_LOG(LogMonsters, VeryVerbose, TEXT("Next condition check set for %f seconds."), NextCheckTimeSeconds);
	FTimerDelegate NextSpawnCheckTimerDelegate;
	NextSpawnCheckTimerDelegate.BindUObject(this, &AMonsterSpawnHandler::CheckSpawnConditionsForAllPlayers);
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

void AMonsterSpawnHandler::CheckSpawnConditionsForPlayer(APawn* Player)
{
	if (Player == nullptr)
	{
		UE_LOG(LogMonsters, Warning, TEXT("Tried to check animal spawn conditions for player, but the player was nullptr."))
		return;
	}

	// Check how many animals are in range of this component
	const int32 MonstersInRange = GetNumMonstersWithinRadiusFromLocation(Player->GetActorLocation());
	UE_LOG(LogMonsters, VeryVerbose, TEXT("%i monsters found in range of player."), MonstersInRange);

	// If no animals are present, there is a chance we will spawn some
	if (MonstersInRange != 0 || !UKismetMathLibrary::RandomBoolWithWeight(0.1f))
	{
		UE_LOG(LogMonsters, Verbose, TEXT("Monster spawn condition not met."));
		return;
	}

	SpawnMonstersInRadiusFromOrigin(Player->GetActorLocation());
}

int32 AMonsterSpawnHandler::GetNumMonstersWithinRadiusFromLocation(const FVector& TestLocation) const
{
	if (SpawnedMonsters.IsEmpty())
	{
		return 0;
	}

	int32 MonstersInRange = 0;

	for (AMonster* Monster : SpawnedMonsters)
	{
		if (Monster == nullptr || FVector::Distance(Monster->GetActorLocation(), TestLocation) > OUTER_SPAWN_RADIUS_CENTIMETERS)
		{
			continue;
		}
		++MonstersInRange;
	}

	return MonstersInRange;
}

void AMonsterSpawnHandler::SpawnMonstersInRadiusFromOrigin(const FVector& SpawnOrigin)
{
	if (MonsterSpawnDataTable == nullptr)
	{
		return;
	}

	TArray<FMonsterSpawnData*> SpawnData;
	static const FString MonsterSpawnDataContextString = TEXT("MonsterSpawnData Context String");

	MonsterSpawnDataTable->GetAllRows(MonsterSpawnDataContextString, SpawnData);

	int32 MonsterToSpawn = FMath::RandRange(0, SpawnData.Num() - 1);

	UE_LOG(LogMonsters, VeryVerbose, TEXT("Spawning monster with ID %i"), MonsterToSpawn);
	for (int32 i = 0; i < SpawnData[MonsterToSpawn]->SpawnGroupSize; ++i)
	{
		AMonster* SpawnedMonster= GetWorld()->SpawnActor<AMonster>(SpawnData[MonsterToSpawn]->Class, GetSpawnTransform(SpawnOrigin));
		SpawnedMonsters.Add(SpawnedMonster);
	}
}

FTransform AMonsterSpawnHandler::GetSpawnTransform(const FVector& SpawnOrigin) const
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

FMonsterSpawnData* AMonsterSpawnHandler::GetSpawnData(const FName& MonsterName)
{
	if (MonsterSpawnDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString = TEXT("MonsterSpawnData Context");
	return MonsterSpawnDataTable->FindRow<FMonsterSpawnData>(MonsterName, ContextString, true);
}
