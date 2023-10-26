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

		const float MaxRange = 10000.0f;
		TArray<AActor*> NodesWithinRange = FilterActorsByRange(AllNodesInWorld, Pawn->GetActorLocation(), MaxRange);
		if (NodesWithinRange.Num() > GenerationSettings.MinNodeCount)
		{
			UE_LOG(LogWorldGeneration, Verbose, TEXT("Found %i Nodes Around Player, Less than %i is required to invoke regeneration."), NodesWithinRange.Num(), GenerationSettings.MinNodeCount);
			continue;
		}

		RegenerateNodesAroundOrigin(GenerationSettings, Pawn->GetActorLocation());
	}
}

void UResourceRegenerationComponent::RegenerateNodesAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const FVector& Origin)
{
	const int32 AmountOfNodesToSpawn = FMath::RandRange(1, 10);

	FBiomeGenerationData* BiomeData = AWorldGenerationHandler::GetBiomeGenerationData(TEXT("Plains"));

	for (int32 i = 0; i < AmountOfNodesToSpawn; i++)
	{
		const int32 NodeIndexToSpawn = FMath::RandRange(0, BiomeData->Nodes.Num() - 1);
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

		GetWorld()->SpawnActor<AActor>(BiomeData->Nodes[NodeIndexToSpawn].BlueprintClass, SpawnTransform);
		UE_LOG(LogWorldGeneration, VeryVerbose, TEXT("Spawned Node Index: %i, At Location: %s"), NodeIndexToSpawn, *SpawnTransform.GetLocation().ToString());
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