// Copyright Telephone Studios. All Rights Reserved.


#include "AISpawnManager.h"
#include "Characters/WildOmissionAICharacter.h"
#include "Structs/AISpawnData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

// Sets default values
AAISpawnManager::AAISpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MinSpawnCheckTimeSeconds = 5.0f;
	MaxSpawnCheckTimeSeconds = 15.0f;
	OuterSpawnRadiusCentimeters = 10000.0f;
	InnerSpawnRadiusCentimeters = 5000.0f;
	SpawnChance = 1.0f;

	SpawnsAll = false;

	AISpawnDataTable = nullptr;
}

// Called when the game starts or when spawned
void AAISpawnManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnedAICharacters.Empty();

	CheckSpawnConditionsForAllPlayers();
}

bool AAISpawnManager::IsSpawnConditionValid()
{
	return true;
}

void AAISpawnManager::CheckSpawnConditionsForAllPlayers()
{
	UE_LOG(LogWildOmissionAI, Verbose, TEXT("Checking AICharacter Spawn Conditions."));
	
	// Set timer to call this function again in the future
	const float NextCheckTimeSeconds = FMath::RandRange(MinSpawnCheckTimeSeconds, MaxSpawnCheckTimeSeconds);
	UE_LOG(LogWildOmissionAI, VeryVerbose, TEXT("Making Next AICharacter Spawn Check In %f Seconds."), NextCheckTimeSeconds);
	FTimerDelegate NextSpawnCheckTimerDelegate;
	NextSpawnCheckTimerDelegate.BindUObject(this, &AAISpawnManager::CheckSpawnConditionsForAllPlayers);
	GetWorld()->GetTimerManager().SetTimer(NextSpawnCheckTimerHandle, NextSpawnCheckTimerDelegate, NextCheckTimeSeconds, false);

	if (!IsSpawnConditionValid())
	{
		return;
	}

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

void AAISpawnManager::CheckSpawnConditionsForPlayer(APawn* Player)
{
	if (Player == nullptr)
	{
		UE_LOG(LogWildOmissionAI, Warning, TEXT("Tried to check AI spawn conditions for player, but the player was nullptr."))
		return;
	}

	// Check how many AICharacter are in range of this component
	const int32 AICharactersInRange = GetNumAICharactersWithinRadiusFromLocation(Player->GetActorLocation());
	UE_LOG(LogWildOmissionAI, VeryVerbose, TEXT("%i AICharacters Found In Range Of Player."), AICharactersInRange);

	// If no AICharacters are present, there is a chance we will spawn some
	if (AICharactersInRange != 0 || !UKismetMathLibrary::RandomBoolWithWeight(SpawnChance))
	{
		UE_LOG(LogWildOmissionAI, Verbose, TEXT("AICharacter Spawn Conditions Not Met."));
		return;
	}

	SpawnAICharactersInRadiusFromOrigin(Player->GetActorLocation());
}

int32 AAISpawnManager::GetNumAICharactersWithinRadiusFromLocation(const FVector& TestLocation) const
{
	if (SpawnedAICharacters.IsEmpty())
	{
		return 0;
	}

	int32 AICharactersInRange = 0;

	for (AWildOmissionAICharacter* AICharacter : SpawnedAICharacters)
	{
		if (AICharacter == nullptr || FVector::Distance(AICharacter->GetActorLocation(), TestLocation) > OuterSpawnRadiusCentimeters)
		{
			continue;
		}
		++AICharactersInRange;
	}

	return AICharactersInRange;
}

void AAISpawnManager::SpawnAICharactersInRadiusFromOrigin(const FVector& Origin)
{
	if (AISpawnDataTable == nullptr)
	{
		return;
	}

	TArray<FAISpawnData*> SpawnData;
	static const FString AISpawnDataContextString = TEXT("AISpawnData Context String");

	AISpawnDataTable->GetAllRows(AISpawnDataContextString, SpawnData);

	if (SpawnsAll)
	{
		for (FAISpawnData* AIData : SpawnData)
		{
			for (int32 i = 0; i < AIData->SpawnGroupSize; ++i)
			{
				FTransform SpawnTransform;
				if (!FindSpawnTransform(Origin, SpawnTransform))
				{
					continue;
				}

				(void*)HandleAISpawn(AIData->Class, SpawnTransform);
			}
		}
	}
	else
	{
		int32 AICharacterToSpawn = FMath::RandRange(0, SpawnData.Num() - 1);

		if (!SpawnData.IsValidIndex(AICharacterToSpawn))
		{
			return;
		}

		UE_LOG(LogWildOmissionAI, VeryVerbose, TEXT("Spawning AICharacter With An Index Of %i."), AICharacterToSpawn);
		for (int32 i = 0; i < SpawnData[AICharacterToSpawn]->SpawnGroupSize; ++i)
		{
			FTransform SpawnTransform;
			if (!FindSpawnTransform(Origin, SpawnTransform))
			{
				continue;
			}

			(void*)HandleAISpawn(SpawnData[AICharacterToSpawn]->Class, SpawnTransform);
		}
	}
}

void AAISpawnManager::RemoveAICharacterFromList(AWildOmissionAICharacter* CharacterToRemove)
{
	SpawnedAICharacters.Remove(CharacterToRemove);
}

bool AAISpawnManager::FindSpawnTransform(const FVector& Origin, FTransform& OutTransform) const
{
	const float TraceHeight = 50000.0f;
	const float SpawnDistance = FMath::RandRange(InnerSpawnRadiusCentimeters, OuterSpawnRadiusCentimeters);
	const float SpawnAngle = FMath::RandRange(0.0f, 360.0f);

	FVector SpawnLocationWithinRadius = FVector::ForwardVector * SpawnDistance;
	SpawnLocationWithinRadius = SpawnLocationWithinRadius.RotateAngleAxis(SpawnAngle, FVector::UpVector);

	FVector Start = SpawnLocationWithinRadius + Origin;
	Start.Z = TraceHeight;
	FVector End = Start - FVector(0.0f, 0.0f, TraceHeight);

	FHitResult HitResult;
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		return false;
	}

	FRotator SpawnRotation = FRotator::ZeroRotator;
	SpawnRotation.Yaw = FMath::RandRange(0.0f, 360.0f);

	OutTransform.SetLocation(HitResult.ImpactPoint + FVector(0.0f, 0.0f, 100.0f));
	OutTransform.SetRotation(FQuat(SpawnRotation));

	return true;
}

FAISpawnData* AAISpawnManager::GetSpawnData(const FName& AIName)
{
	if (AISpawnDataTable== nullptr)
	{
		return nullptr;
	}

	static const FString ContextString = TEXT("AISpawnData Context");
	return AISpawnDataTable->FindRow<FAISpawnData>(AIName, ContextString, true);
}

AWildOmissionAICharacter* AAISpawnManager::HandleAISpawn(UClass* Class, const FTransform& SpawnTransform)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	AWildOmissionAICharacter* SpawnedAICharacter = World->SpawnActor<AWildOmissionAICharacter>(Class, SpawnTransform);
	if (SpawnedAICharacter == nullptr)
	{
		return nullptr;
	}

	SpawnedAICharacter->OnDespawn.AddDynamic(this, &AAISpawnManager::RemoveAICharacterFromList);
	SpawnedAICharacters.Add(SpawnedAICharacter);
	return SpawnedAICharacter;
}
