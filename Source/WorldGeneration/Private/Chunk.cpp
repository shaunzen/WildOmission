// Copyright Telephone Studios. All Rights Reserved.


#include "Chunk.h"
#include "WorldGenerationHandler.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	Size = 16;

	ZScale = 100.0f;
	NoiseScale = 0.1f;

	Scale = 100.0f;
	UVScale = 1.0f;
	Material = nullptr;

	Tags.Add(TEXT("Ground"));
}

void AChunk::Generate()
{
	GenerateTerrain();
	GenerateTrees();
	GenerateNodes();
}

uint32 AChunk::GetSize() const
{
	return Size;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	Generate();
}

void AChunk::GenerateTerrain()
{
	Verticies.Reset();
	Triangles.Reset();
	UV0.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateVerticies();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Verticies, Triangles, UV0, Normals, Tangents);

	MeshComponent->CreateMeshSection(0, Verticies, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	MeshComponent->SetMaterial(0, Material);
}

void AChunk::GenerateTrees()
{
	FBiomeGenerationData* Biome = AWorldGenerationHandler::GetBiomeGenerationData(TEXT("Plains"));
	if (Biome == nullptr)
	{
		return;
	}
	
	for (const FSpawnData& Resource : Biome->Trees)
	{
		const int32 AmountOfResourceToSpawn = FMath::RoundToInt32((Size * Resource.DensityPerMeter) / Biome->Trees.Num());
		for (int32 i = 0; i < AmountOfResourceToSpawn; i++)
		{
			FTransform SpawnTransform;
			if (!GetRandomPointOnTerrain(SpawnTransform))
			{
				continue;
			}

			AActor* SpawnedResource = GetWorld()->SpawnActor<AActor>(Resource.BlueprintClass, SpawnTransform);
			Trees.Add(SpawnedResource);
		}
	}
}

void AChunk::GenerateNodes()
{
	FBiomeGenerationData* Biome = AWorldGenerationHandler::GetBiomeGenerationData(TEXT("Plains"));
	if (Biome == nullptr)
	{
		return;
	}

	for (const FSpawnData& Resource : Biome->Nodes)
	{
		const int32 AmountOfResourceToSpawn = FMath::RoundToInt32((Size * Resource.DensityPerMeter) / Biome->Nodes.Num());
		for (int32 i = 0; i < AmountOfResourceToSpawn; i++)
		{
			FTransform SpawnTransform;
			if (!GetRandomPointOnTerrain(SpawnTransform))
			{
				continue;
			}

			AActor* SpawnedResource = GetWorld()->SpawnActor<AActor>(Resource.BlueprintClass, SpawnTransform);
			Nodes.Add(SpawnedResource);
		}
	}
}

bool AChunk::GetRandomPointOnTerrain(FTransform& OutTransform)
{
	const int32 Point = FMath::RandRange(0, Verticies.Num() - 1);
	if (!Verticies.IsValidIndex(Point))
	{
		return false;
	}

	OutTransform.SetLocation(Verticies[Point] + GetActorLocation());
	return true;
}

void AChunk::CreateVerticies()
{
	const uint32 Seed = FMath::RandRange(0, 1000000);
	for (uint32 X = 0; X <= Size; ++X)
	{
		for (uint32 Y = 0; Y <= Size; ++Y)
		{
			const float Z = FMath::PerlinNoise2D(FVector2D(static_cast<float>(X + GetActorLocation().X + Seed) * NoiseScale, static_cast<float>(Y + GetActorLocation().Y + Seed) * NoiseScale)) * ZScale;
			Verticies.Add(FVector(X * Scale, Y * Scale, Z));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void AChunk::CreateTriangles()
{
	uint32 Vertex = 0;
	for (uint32 X = 0; X < Size; ++X)
	{
		for (uint32 Y = 0; Y < Size; ++Y)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + Size + 1);

			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + Size + 2);
			Triangles.Add(Vertex + Size + 1);

			++Vertex;
		}
		++Vertex;
	}
}
