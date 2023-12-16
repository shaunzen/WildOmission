// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Chunk.h"
#include "ChunkManager.h"
#include "Structs/SavableObjectDefinition.h"
#include "Interfaces/SavableObject.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Noise/PerlinNoise.hpp"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "UObject/ConstructorHelpers.h"

static siv::PerlinNoise Noise(10);
const static int32 VERTEX_SIZE = 16;
const static float VERTEX_DISTANCE_SCALE = 100.0f;

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	// MeshComponent->SetupAttachment(RootComponent);

	// TODO these might be tied to biome in the future
	ZScale = 100.0f;
	NoiseScale = 0.1f;

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

void AChunk::Save(FChunkData& OutChunkData, bool AlsoDestroy)
{
	OutChunkData.GridLocation = GetChunkLocation();
	OutChunkData.Generated = true;
	
	FMemoryWriter ChunkMemoryWriter(OutChunkData.ByteData);
	FObjectAndNameAsStringProxyArchive ChunkArchive(ChunkMemoryWriter, true);
	ChunkArchive.ArIsSaveGame = true;
	this->Serialize(ChunkArchive);

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* AttachedActor : AttachedActors)
	{
		if (!IsValid(AttachedActor) || !AttachedActor->Implements<USavableObject>())
		{
			continue;
		}
		
		ISavableObject* SavableObjectActor = Cast<ISavableObject>(AttachedActor);
		if (SavableObjectActor == nullptr)
		{
			UE_LOG(LogWorldGeneration, Warning, TEXT("Cannot Cast to SavableObject, Actor: %s"), *Actor->GetActorNameOrLabel());
			continue;
		}
		
		FActorSaveData ActorSaveData;
		ActorSaveData.Identifier = SavableObjectActor->GetIdentifier();
		ActorSaveData.Transform = AttachedActor->GetTransform();
		
		FMemoryWriter ActorMemoryWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ActorArchive(ActorMemoryWriter, true);
		ActorArchive.ArIsSaveGame = true;
		AttachedActor->Serialize(ActorArchive);

		TArray<UActorComponent*> SavableComponents = AttachedActor->GetComponentsByInterface(USavableObject::StaticClass());
		for (UActorComponent* ActorComponent : SavableComponents)
		{
			FActorComponentSaveData ComponentSaveData;
			ComponentSaveData.Name = ActorComponent->GetFName();
			ComponentSaveData.Class = ActorComponent->GetClass();
			
			FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentArchive(ComponentMemoryWriter, true);
			ComponentArchive.ArIsSaveGame = true;

			ActorComponent->Serialize(ComponentArchive);

			ActorSaveData.ComponentData.Add(ComponentSaveData);
		}

		OutChunkData.ActorData.Add(ActorSaveData);
	}
}

void AChunk::SetGenerationSeed(const uint32& Seed)
{
	Noise.reseed(Seed);
}

float AChunk::GetTerrainHeightAtLocation(const FVector2D& Location, float VertexDistanceScale)
{
	const float MajorScale = 0.005f;
	const float MinorScale = 0.1f;
	const float RoughnessScale = 0.2f;
	const float MajorHeight = Noise.noise2D(Location.X * MajorScale, Location.Y * MajorScale) * 10000.0f;
	const float MinorHeight = Noise.noise2D(Location.X * MinorScale, Location.Y * MinorScale) * 100.0f;
	const float RoughnessHeight = Noise.octave2D(Location.X * RoughnessScale, Location.Y * RoughnessScale, 3) * 10.0f;

	return MajorHeight + MinorHeight + RoughnessHeight;
	//const float NewZ = Noise.octave2D(static_cast<float>(X + Location.X) * NoiseScale, static_cast<float>(Y + Location.Y) * NoiseScale, 3) * ZScale;
}

FIntVector2 AChunk::GetChunkLocation() const
{
	return GridLocation;
}

int32 AChunk::GetVertexSize()
{
	return VERTEX_SIZE;
}

float AChunk::GetVertexDistanceScale()
{
	return VERTEX_DISTANCE_SCALE;
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
		const int32 AmountOfResourceToSpawn = FMath::RoundToInt32((VERTEX_SIZE * SpawnData.DensityPerMeter) / SpawnDataList.Num());
		for (int32 i = 0; i < AmountOfResourceToSpawn; i++)
		{
			FTransform SpawnTransform;
			if (!GetRandomPointOnTerrain(SpawnTransform))
			{
				continue;
			}
			const float Yaw = FMath::RandRange(0.0f, 360.0f);
			SpawnTransform.SetRotation(FRotator(0.0f, Yaw, 0.0f).Quaternion());
			AActor* SpawnedResource = GetWorld()->SpawnActor<AActor>(SpawnData.BlueprintClass, SpawnTransform);
			SpawnedResource->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AChunk::CreateVerticies()
{
	const FVector Location = GetActorLocation() * 0.01f;
	for (uint32 X = 0; X <= VERTEX_SIZE; ++X)
	{
		for (uint32 Y = 0; Y <= VERTEX_SIZE; ++Y)
		{
			//const float NewZ = Noise.octave2D(static_cast<float>(X + Location.X) * NoiseScale, static_cast<float>(Y + Location.Y) * NoiseScale, 3) * ZScale;
			//const float Z = FMath::PerlinNoise2D(FVector2D((static_cast<float>(X) + Location.X) * NoiseScale, (static_cast<float>(Y) + Location.Y) * NoiseScale)) * ZScale;
			const float Z = GetTerrainHeightAtLocation(FVector2D(X + Location.X, Y + Location.Y));

			const float Offset = (VERTEX_SIZE * VERTEX_DISTANCE_SCALE) * 0.5f;

			Verticies.Add(FVector((X * VERTEX_DISTANCE_SCALE) - Offset, (Y * VERTEX_DISTANCE_SCALE) - Offset, Z));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void AChunk::CreateTriangles()
{
	uint32 Vertex = 0;
	for (uint32 X = 0; X < VERTEX_SIZE; ++X)
	{
		for (uint32 Y = 0; Y < VERTEX_SIZE; ++Y)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + VERTEX_SIZE + 1);

			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + VERTEX_SIZE + 2);
			Triangles.Add(Vertex + VERTEX_SIZE + 1);

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
