// Copyright Telephone Studios. All Rights Reserved.


#include "ResourceRegenerationComponent.h"
#include "WorldGenerationHandler.h"
#include "Structs/BiomeGenerationData.h"
#include "Structs/WorldGenerationSettings.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Log.h"

// Sets default values for this component's properties
UResourceRegenerationComponent::UResourceRegenerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InnerRegenerationRadius = 5000.0f;
	OuterRegenerationRadius = 10000.0f;
}

// Called when the game starts
void UResourceRegenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle NodeRegenerationTimerHandle;
	FTimerDelegate NodeRegenerationTimerDelegate;
	
	NodeRegenerationTimerDelegate.BindUObject(this, &UResourceRegenerationComponent::CheckNodeRegenerationConditions);
	GetWorld()->GetTimerManager().SetTimer(NodeRegenerationTimerHandle, NodeRegenerationTimerDelegate, 60.0f, true);
}

void UResourceRegenerationComponent::CheckNodeRegenerationConditions()
{
	UE_LOG(LogWorldGeneration, Verbose, TEXT("Checking Node Regeneration Conditions."));
	FWorldGenerationSettings GenerationSettings;

	TArray<AActor*> AllNodesInWorld;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Node"), AllNodesInWorld);

	// We only want to do this around players
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			continue;
		}

		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn == nullptr)
		{
			continue;
		}

		CheckNodeRegenerationConditionsAroundOrigin(GenerationSettings, AllNodesInWorld, Pawn->GetActorLocation());
	}
}

void UResourceRegenerationComponent::CheckNodeRegenerationConditionsAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const TArray<AActor*> AllNodesInWorld, const FVector& Origin)
{
	FBiomeGenerationData* BiomeData = AWorldGenerationHandler::GetBiomeGenerationData(TEXT("Plains"));
	if (BiomeData == nullptr)
	{
		return;
	}

	const float MaxRange = 10000.0f;
	TArray<AActor*> NodesWithinRange = FilterActorsByRange(AllNodesInWorld, Origin, MaxRange);
	
	// Count Each Node That Can Spawn
	for (const FSpawnData& NodeSpawnData : BiomeData->Nodes)
	{
		int32 NodeCount = 0;
		for (AActor* Node : NodesWithinRange)
		{
			if (Node->StaticClass() != NodeSpawnData.BlueprintClass)
			{
				continue;
			}

			++NodeCount;
		}
		
		if (NodeCount > 2)
		{
			UE_LOG(LogWorldGeneration, Verbose, TEXT("Found %i Nodes Around Player, Less than 2 is required to invoke regeneration."), NodeCount);
			continue;
		}

		RegenerateNodeAroundOrigin(GenerationSettings, NodeSpawnData.BlueprintClass, Origin);
	}
}

void UResourceRegenerationComponent::RegenerateNodeAroundOrigin(const FWorldGenerationSettings& GenerationSettings, UClass* NodeClass, const FVector& Origin)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const int32 AmountOfNodesToSpawn = FMath::RandRange(1, 10);

	for (int32 i = 0; i < AmountOfNodesToSpawn; i++)
	{
		FTransform SpawnTransform;
		if (!AWorldGenerationHandler::FindSpawnTransformRadiusFromOrigin(GetWorld(), SpawnTransform, Origin, 
			InnerRegenerationRadius, OuterRegenerationRadius, GenerationSettings, true))
		{
			continue;
		}

		if (IsTransformWithinInnerRangeOfAnyPlayer(SpawnTransform))
		{
			continue;
		}

		AActor* SpawnedNode = World->SpawnActor<AActor>(NodeClass, SpawnTransform);
		if (SpawnedNode == nullptr)
		{
			continue;
		}

		UE_LOG(LogWorldGeneration, VeryVerbose, TEXT("Spawned Node: %s, At Location: %s"), *SpawnedNode->GetActorNameOrLabel(), *SpawnTransform.GetLocation().ToString());
	}
}

bool UResourceRegenerationComponent::IsTransformWithinInnerRangeOfAnyPlayer(const FTransform& TransformToCheck) const
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			continue;
		}

		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn == nullptr)
		{
			continue;
		}

		const FVector PlayerLocation = Pawn->GetActorLocation();
		const FVector TransformLocation = TransformToCheck.GetLocation();
		const float Distance = FVector::Distance(PlayerLocation, TransformLocation);

		if (Distance < InnerRegenerationRadius)
		{
			return true;
		}
	}

	return false;
}

TArray<AActor*> UResourceRegenerationComponent::FilterActorsByRange(const TArray<AActor*>& InList, const FVector& Origin, float Range)
{
	TArray<AActor*> FilteredList;
	for (AActor* InActor : InList)
	{
		if (InActor == nullptr)
		{
			continue;
		}

		const float Distance = FVector::Distance(InActor->GetActorLocation(), Origin);
		if (Distance > Range)
		{
			continue;
		}

		FilteredList.Add(InActor);
	}

	return FilteredList;
}