// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Chunk.h"
#include "ProceduralMeshComponent.h"
#include "Components/ChunkSaveComponent.h"
#include "ChunkManager.h"
#include "Curves/CurveFloat.h"
#include "Noise/PerlinNoise.hpp"
#include "KismetProceduralMeshLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

static siv::PerlinNoise Noise(10);
const static int32 VERTEX_SIZE = 16;
const static float VERTEX_DISTANCE_SCALE = 100.0f;
const static float MAX_HEIGHT = 1000000.0f;
static UCurveFloat* ContinentalnessHeightCurve = nullptr;;
static UCurveFloat* ErosionHeightCurve = nullptr;
static UCurveFloat* PeaksAndValleysHeightCurve = nullptr;

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	// MeshComponent->SetupAttachment(RootComponent);

	SaveComponent = CreateDefaultSubobject<UChunkSaveComponent>(TEXT("SaveComponent"));

	// TODO these might be tied to biome in the future
	ZScale = 100.0f;
	NoiseScale = 0.1f;

	UVScale = 100.0f;
	Material = nullptr;

	if (GetWorld())
	{
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> TerrainMaterial(TEXT("/Game/WorldGeneration/M_Terrain"));
		if (TerrainMaterial.Succeeded())
		{
			Material = TerrainMaterial.Object;
		}

		static ConstructorHelpers::FObjectFinder<UCurveFloat> ContinentalnessHeightCurveBlueprint(TEXT("/Game/WorldGeneration/Curves/Curve_Continentalness_Height"));
		if (ContinentalnessHeightCurveBlueprint.Succeeded())
		{
			ContinentalnessHeightCurve = ContinentalnessHeightCurveBlueprint.Object;
		}

		static ConstructorHelpers::FObjectFinder<UCurveFloat> ErosionHeightCurveBlueprint(TEXT("/Game/WorldGeneration/Curves/Curve_Erosion_Height"));
		if (ErosionHeightCurveBlueprint.Succeeded())
		{
			ErosionHeightCurve = ErosionHeightCurveBlueprint.Object;
		}

		static ConstructorHelpers::FObjectFinder<UCurveFloat> PeaksAndValleysHeightCurveBlueprint(TEXT("/Game/WorldGeneration/Curves/Curve_PeaksAndValleys_Height"));
		if (PeaksAndValleysHeightCurveBlueprint.Succeeded())
		{
			PeaksAndValleysHeightCurve = PeaksAndValleysHeightCurveBlueprint.Object;
		}
	}

	Tags.Add(TEXT("Ground"));
}

void AChunk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AChunk, Verticies, COND_InitialOnly);
}

void AChunk::Generate()
{
	GenerateTerrainShape();
	GenerateBiome();
	GenerateDecorations();
	/*FBiomeGenerationData* Biome = AChunkManager::GetBiomeGenerationData(TEXT("Plains"));
	if (Biome)
	{
		GenerateSpawnableActors(Biome->Trees);
		GenerateSpawnableActors(Biome->Nodes);
		GenerateSpawnableActors(Biome->Collectables);
		GenerateSpawnableActors(Biome->Lootables);
	}*/
}

void AChunk::OnLoadFromSaveComplete()
{
	OnRep_Verticies();
}

void AChunk::Save(FChunkData& OutChunkData, bool AlsoDestroy)
{
	SaveComponent->Save(OutChunkData, AlsoDestroy);
}

void AChunk::Load(const FChunkData& InChunkData)
{
	SaveComponent->Load(InChunkData);
	OnLoadFromSaveComplete();
}

void AChunk::SetGenerationSeed(const uint32& Seed)
{
	Noise.reseed(Seed);
}

float AChunk::GetTerrainHeightAtLocation(const FVector2D& Location, float VertexDistanceScale)
{
	

	
	const float ContinentalnessInfluence = 500.0f;
	const float ErosionInfluence = 10000.0f;
	const float PeaksAndValleysInfluence = 10000.0f;

	const float Continentalness = GetContinentalnessAtLocation(Location);
	const float Erosion = GetErosionAtLocation(Location);
	const float PeaksAndValleys = GetPeaksAndValleysAtLocation(Location);
	const float ContinentalnessHeight = Continentalness * ContinentalnessInfluence;
	const float ErosionHeight = Erosion + 1 * ErosionInfluence;
	const float PeaksAndValleysHeight = PeaksAndValleys * PeaksAndValleysInfluence;
	// If continentalness > 0
		// Add Erosion
	
	/*UE_LOG(LogTemp, Warning, TEXT("Location %s"), *Location.ToString());
	UE_LOG(LogTemp, Warning, TEXT("C %f"), Continentalness);
	UE_LOG(LogTemp, Warning, TEXT("E %f"), Erosion);
	UE_LOG(LogTemp, Warning, TEXT("P&V %f"), PeaksAndValleys);*/
	float Height = ContinentalnessHeight;
	
	if (Continentalness > 0.0f)
	{	
		Height += ErosionHeight * Continentalness;
		if (Erosion > 0.0f)
		{
			Height += PeaksAndValleysHeight * Erosion;
		}
	}
	return Height;
	//const float NewZ = Noise.octave2D(static_cast<float>(X + Location.X) * NoiseScale, static_cast<float>(Y + Location.Y) * NoiseScale, 3) * ZScale;
}

void AChunk::SetChunkLocation(const FIntVector2& InLocation)
{
	GridLocation = InLocation;
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

float AChunk::GetMaxHeight()
{
	return MAX_HEIGHT;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

}

void AChunk::OnRep_Verticies()
{
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateMesh();
}

void AChunk::GenerateTerrainShape()
{
	Verticies.Reset();
	Triangles.Reset();
	UV0.Reset();
	VertexColors.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateVerticies();
	CreateMesh();	
}

void AChunk::GenerateBiome()
{
}

void AChunk::GenerateDecorations()
{
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

float AChunk::GetContinentalnessAtLocation(const FVector2D& Location)
{
	if (ContinentalnessHeightCurve == nullptr)
	{
		return 0.0f;
	}

	const float ContinentalnessScale = 0.0001f;
	return ContinentalnessHeightCurve->GetFloatValue(
		Noise.noise2D(Location.X * ContinentalnessScale, Location.Y * ContinentalnessScale));
}

float AChunk::GetErosionAtLocation(const FVector2D& Location)
{
	if (ErosionHeightCurve == nullptr)
	{
		return 0.0f;
	}

	const float ErosionScale = 0.001f;
	const float ErosionOffset = 1000.0f;
	return ErosionHeightCurve->GetFloatValue(
		Noise.noise2D((Location.X * ErosionOffset) * ErosionScale, (Location.Y * ErosionOffset) * ErosionScale));
}

float AChunk::GetPeaksAndValleysAtLocation(const FVector2D& Location)
{
	if (PeaksAndValleysHeightCurve == nullptr)
	{
		return 0.0f;
	}

	const float PeaksAndValleysScale = 0.0005f;
	const float PeaksAndValleysOffset = 5000.0f;
	return PeaksAndValleysHeightCurve->GetFloatValue(
		Noise.noise2D((Location.X * PeaksAndValleysOffset) * PeaksAndValleysScale, (Location.Y * PeaksAndValleysOffset) * PeaksAndValleysScale));
}

void AChunk::CreateVerticies()
{
	const FVector Location = GetActorLocation();
	for (uint32 X = 0; X <= VERTEX_SIZE; ++X)
	{
		for (uint32 Y = 0; Y <= VERTEX_SIZE; ++Y)
		{
			//const float NewZ = Noise.octave2D(static_cast<float>(X + Location.X) * NoiseScale, static_cast<float>(Y + Location.Y) * NoiseScale, 3) * ZScale;
			//const float Z = FMath::PerlinNoise2D(FVector2D((static_cast<float>(X) + Location.X) * NoiseScale, (static_cast<float>(Y) + Location.Y) * NoiseScale)) * ZScale;
			const float Z = GetTerrainHeightAtLocation(FVector2D((X * VERTEX_DISTANCE_SCALE) + Location.X, (Y * VERTEX_DISTANCE_SCALE) + Location.Y));

			const float Offset = (VERTEX_SIZE * VERTEX_DISTANCE_SCALE) * 0.5f;

			Verticies.Add(FVector((X * VERTEX_DISTANCE_SCALE) - Offset, (Y * VERTEX_DISTANCE_SCALE) - Offset, Z));
			VertexColors.Add(FColor::Green);
			UV0.Add(FVector2D(X * 0.0625f, Y * 0.0625f));
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
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Verticies, Triangles, UV0, Normals, Tangents);

	MeshComponent->CreateMeshSection(0, Verticies, Triangles, Normals, UV0, VertexColors, Tangents, true);
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

FBiomeGenerationData* AChunk::GetBiomeAtLocation(const FVector2D& Location) const
{
	const float TempatureScale = 0.01f;
	const float HumidityScale = 0.001f;

	const float Tempature = Noise.noise2D(Location.X * TempatureScale, Location.Y * TempatureScale);
	const float Humidity = Noise.noise2D(Location.X * HumidityScale, Location.Y * TempatureScale);

	// Temp High, Humidity Low = Desert
	// Temp High, Humidity High = Swamp
	// Temp Low, Humidity Low = Tundra
	// Temp Low, Humidity High = Snow Forest
	// Temp ~, Humidity ~ = Plains/Forest

	return nullptr;
}
