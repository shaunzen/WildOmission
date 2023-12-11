// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Chunk.h"
#include "ChunkManager.h"
#include "Noise/PerlinNoise.hpp"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "UObject/ConstructorHelpers.h"

static siv::PerlinNoise Noise(10);

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	// MeshComponent->SetupAttachment(RootComponent);

	Size = 16;

	ZScale = 100.0f;
	NoiseScale = 0.1f;

	Scale = 100.0f;
	UVScale = 1.0f;
	Material = nullptr;

	if (GetWorld())
	{
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> TerrainMaterial(TEXT("/Game/WorldGeneration/M_Terrain"));
		if (TerrainMaterial.Succeeded())
		{
			Material = TerrainMaterial.Object;
		}
	}

	Tags.Add(TEXT("Ground"));
}

void AChunk::Generate(const FIntVector2& InLocation)
{
	GridLocation = InLocation;
	GenerateTerrain();

	FBiomeGenerationData* Biome = AChunkManager::GetBiomeGenerationData(TEXT("Plains"));
	if (Biome)
	{
		GenerateSpawnableActors(Biome->Trees);
		GenerateSpawnableActors(Biome->Nodes);
		GenerateSpawnableActors(Biome->Collectables);
		GenerateSpawnableActors(Biome->Lootables);
	}
}

void AChunk::OnLoadFromSaveComplete()
{
	UV0.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateMesh();
}

void AChunk::SetGenerationSeed(const uint32& Seed)
{
	Noise.reseed(Seed);
}

FIntVector2 AChunk::GetChunkLocation() const
{
	return GridLocation;
}

uint32 AChunk::GetSize() const
{
	return Size;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

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
	CreateMesh();	
}

void AChunk::GenerateSpawnableActors(const TArray<struct FSpawnData>& SpawnDataList)
{
	for (const FSpawnData& SpawnData : SpawnDataList)
	{
		const int32 AmountOfResourceToSpawn = FMath::RoundToInt32((Size * SpawnData.DensityPerMeter) / SpawnDataList.Num());
		for (int32 i = 0; i < AmountOfResourceToSpawn; i++)
		{
			FTransform SpawnTransform;
			if (!GetRandomPointOnTerrain(SpawnTransform))
			{
				continue;
			}

			AActor* SpawnedResource = GetWorld()->SpawnActor<AActor>(SpawnData.BlueprintClass, SpawnTransform);
			SpawnedResource->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AChunk::CreateVerticies()
{
	const uint32 Seed = 10; //= FMath::RandRange(0, 1000000);
	const FVector Location = GetActorLocation() * 0.01f;
	for (uint32 X = 0; X <= Size; ++X)
	{
		for (uint32 Y = 0; Y <= Size; ++Y)
		{
			const float NewZ = Noise.octave2D(static_cast<float>(X + Location.X) * NoiseScale, static_cast<float>(Y + Location.Y) * NoiseScale, 3) * ZScale;
			//const float Z = FMath::PerlinNoise2D(FVector2D((static_cast<float>(X) + Location.X) * NoiseScale, (static_cast<float>(Y) + Location.Y) * NoiseScale)) * ZScale;
			Verticies.Add(FVector(X * Scale, Y * Scale, NewZ));
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

void AChunk::CreateMesh()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Verticies, Triangles, UV0, Normals, Tangents);

	MeshComponent->CreateMeshSection(0, Verticies, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	MeshComponent->SetMaterial(0, Material);
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
