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
static uint32 Seed = 0;
const static int32 VERTEX_SIZE = 16;
const static float VERTEX_DISTANCE_SCALE = 100.0f;
const static float MAX_HEIGHT = 1000000.0f;
static UCurveFloat* ContinentalnessHeightCurve = nullptr;;
static UCurveFloat* ErosionHeightCurve = nullptr;
static UCurveFloat* PeaksAndValleysHeightCurve = nullptr;

const static FColor SAND_VERTEX_COLOR = FColor(255, 0, 0);
const static FColor GRASS_VERTEX_COLOR = FColor(0, 255, 0);
const static FColor DIRT_VERTEX_COLOR = FColor(0, 0, 255);
const static FColor GRAVEL_VERTEX_COLOR = FColor(255, 255, 0);
const static FColor STONE_VERTEX_COLOR = FColor(0, 255, 255);
const static FColor SNOW_VERTEX_COLOR = FColor(255, 255, 255);
const static FColor RESERVED_VERTEX_COLOR = FColor(0, 0, 0);

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->ComponentTags.Add(TEXT("Ground"));
	RootComponent = MeshComponent;
	// MeshComponent->SetupAttachment(RootComponent);

	WaterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMeshComponent"));
	WaterMeshComponent->ComponentTags.Add(TEXT("Water"));
	WaterMeshComponent->SetupAttachment(MeshComponent);

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
}

void AChunk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AChunk, Vertices, COND_InitialOnly);
}

void AChunk::Generate(const TArray<FChunkData>& Neighbors)
{
	GenerateTerrainShape(Neighbors);
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
	OnRep_Vertices();
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

void AChunk::SetGenerationSeed(const uint32& InSeed)
{
	Seed = InSeed;
	Noise.reseed(InSeed);
}

uint32 AChunk::GetGenerationSeed()
{
	return Seed;
}

void AChunk::GetTerrainDataAtLocation(const FVector2D& Location, float& OutHeight, FColor& OutColor)
{
	const float ContinentalnessInfluence = 1000.0f;
	const float ErosionInfluence = 5000.0f;
	const float PeaksAndValleysInfluence = 25000.0f;

	const float NormalizedContinentalnessHeight = GetContinentalnessAtLocation(Location);
	const float NormalizedErosionHeight = GetErosionAtLocation(Location);
	const float NormalizedPeaksAndValleysHeight = GetPeaksAndValleysAtLocation(Location);

	const float ContinentalnessHeight = NormalizedContinentalnessHeight * ContinentalnessInfluence;
	const float ErosionHeight = (NormalizedContinentalnessHeight + 1) * ErosionInfluence;
	const float PeaksAndValleysHeight = (NormalizedContinentalnessHeight + 1) * PeaksAndValleysInfluence;

	const float RawContinentalness = GetContinentalnessAtLocation(Location, true);
	const float RawErosion = GetErosionAtLocation(Location, true);
	const float RawPeaksAndValleys = GetPeaksAndValleysAtLocation(Location, true);
	
	// Height
	OutHeight = ContinentalnessHeight;
	if (RawContinentalness > 0.0f)
	{	
		OutHeight += ErosionHeight * RawContinentalness;
		if (RawErosion > 0.0f)
		{
			OutHeight += PeaksAndValleysHeight * RawErosion * RawContinentalness;
		}
	}
	OutColor = FColor::Yellow;
	// Ground Color
	if (RawContinentalness <= 0.0f)
	{
		OutColor = SAND_VERTEX_COLOR;
	}
	else
	{	
		const bool HighAltitude = OutHeight > 10000.0f;
		HighAltitude? OutColor = SNOW_VERTEX_COLOR : OutColor = GRASS_VERTEX_COLOR;
	}
}

void AChunk::SetChunkLocation(const FIntVector2& InLocation)
{
	GridLocation = InLocation;
}

FIntVector2 AChunk::GetChunkLocation() const
{
	return GridLocation;
}

void AChunk::SetVertices(const TArray<FVector>& InVertices)
{
	Vertices = InVertices;
}

TArray<FVector> AChunk::GetVertices() const
{
	return Vertices;
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

void AChunk::OnRep_Vertices()
{
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateMesh();
}

void AChunk::GenerateTerrainShape(const TArray<FChunkData>& Neighbors)
{
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();
	VertexColors.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateVertices(Neighbors);
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

float AChunk::GetContinentalnessAtLocation(const FVector2D& Location, bool UseRawValue)
{
	if (ContinentalnessHeightCurve == nullptr)
	{
		return 0.0f;
	}
	
	const float ContinentalnessScale = 0.000005f;
	const float RawValue = Noise.octave2D(Location.X * ContinentalnessScale, Location.Y * ContinentalnessScale, 3);

	return UseRawValue ? RawValue : ContinentalnessHeightCurve->GetFloatValue(RawValue);
}

float AChunk::GetErosionAtLocation(const FVector2D& Location, bool UseRawValue)
{
	if (ErosionHeightCurve == nullptr)
	{
		return 0.0f;
	}

	const float ErosionScale = 0.00005f;
	const float ErosionOffset = 50000.0f;
	const FVector2D TestLocation = Location + ErosionOffset;

	const float RawValue = Noise.octave2D(TestLocation.X * ErosionScale, TestLocation.Y * ErosionScale, 3);

	return UseRawValue ? RawValue : ErosionHeightCurve->GetFloatValue(RawValue);
}

float AChunk::GetPeaksAndValleysAtLocation(const FVector2D& Location, bool UseRawValue)
{
	if (PeaksAndValleysHeightCurve == nullptr)
	{
		return 0.0f;
	}

	const float PeaksAndValleysScale = 0.00005f;
	const float PeaksAndValleysOffset = -50000.0f;
	const FVector2D TestLocation = Location + PeaksAndValleysOffset;
	const float RawValue = Noise.octave2D(TestLocation.X * PeaksAndValleysScale, TestLocation.Y * PeaksAndValleysScale, 3);
	return UseRawValue ? RawValue : PeaksAndValleysHeightCurve->GetFloatValue(RawValue);
}

void AChunk::CreateVertices(const TArray<FChunkData>& Neighbors)
{
	TArray<FVector> TopNeighborVertices;
	TArray<FVector> BottomNeighborVertices;
	TArray<FVector> LeftNeighborVertices;
	TArray<FVector> RightNeighborVertices;
	
	TArray<FVector> TopLeftNeighborVertices;
	TArray<FVector> TopRightNeighborVertices;
	TArray<FVector> BottomLeftNeighborVertices;
	TArray<FVector> BottomRightNeighborVertices;

	// Iterate through neibors and store their edge height
	for (const FChunkData& ChunkData : Neighbors)
	{
		// Read Vertices data
		TArray<FVector> NeighborVertices = ChunkData.Vertices;
		
		// Top
		if (ChunkData.GridLocation == GridLocation + FIntVector2(1, 0))
		{
			TopNeighborVertices = NeighborVertices;
		}
		// Bottom
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(-1, 0))
		{
			BottomNeighborVertices = NeighborVertices;
		}
		// Left
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(0, -1))
		{
			LeftNeighborVertices = NeighborVertices;
		}
		// Right
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(0, 1))
		{
			RightNeighborVertices = NeighborVertices;
		}
		// Top Left
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(1, -1))
		{
			TopLeftNeighborVertices = NeighborVertices;
		}
		// Top Right
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(1, 1))
		{
			TopRightNeighborVertices = NeighborVertices;
		}
		// Bottom Left
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(-1, -1))
		{
			BottomLeftNeighborVertices = NeighborVertices;
		}
		// Bottom Right
		else if (ChunkData.GridLocation == GridLocation + FIntVector2(-1, 1))
		{
			BottomRightNeighborVertices = NeighborVertices;
		}
	}

	// Use Neighbor edge height when generating this to ensure that there will be no gaps

	const FVector Location = GetActorLocation();
	for (uint32 X = 0; X <= VERTEX_SIZE; ++X)
	{
		for (uint32 Y = 0; Y <= VERTEX_SIZE; ++Y)
		{
			//const float NewZ = Noise.octave2D(static_cast<float>(X + Location.X) * NoiseScale, static_cast<float>(Y + Location.Y) * NoiseScale, 3) * ZScale;
			//const float Z = FMath::PerlinNoise2D(FVector2D((static_cast<float>(X) + Location.X) * NoiseScale, (static_cast<float>(Y) + Location.Y) * NoiseScale)) * ZScale;
			float Height;
			FColor TerrainColor;
			GetTerrainDataAtLocation(FVector2D((X * VERTEX_DISTANCE_SCALE) + Location.X, (Y * VERTEX_DISTANCE_SCALE) + Location.Y), Height, TerrainColor);

			const int32 RowColumnSize = VERTEX_SIZE + 1;
			
			// Top
			if (!TopNeighborVertices.IsEmpty() && X == VERTEX_SIZE)
			{
				Height = TopNeighborVertices[Y].Z;
			}
			// Bottom
			else if (!BottomNeighborVertices.IsEmpty() && X == 0)
			{
				Height = BottomNeighborVertices[Y + (RowColumnSize * VERTEX_SIZE)].Z;
			}
			// Left
			else if (!LeftNeighborVertices.IsEmpty() && Y == 0)
			{
				Height = LeftNeighborVertices[(X * RowColumnSize) + VERTEX_SIZE].Z;
			}
			// Right
			else if (!RightNeighborVertices.IsEmpty() && Y == VERTEX_SIZE)
			{

				Height = RightNeighborVertices[X * RowColumnSize].Z;
			}
			// Top Left
			else if (!TopLeftNeighborVertices.IsEmpty() && X == VERTEX_SIZE && Y == 0)
			{
				Height = TopLeftNeighborVertices[VERTEX_SIZE].Z;
			}
			// Top Right
			else if (!TopRightNeighborVertices.IsEmpty() && X == VERTEX_SIZE && Y == VERTEX_SIZE)
			{
				Height = TopRightNeighborVertices[0].Z;
			}
			// Bottom Left
			else if (!BottomLeftNeighborVertices.IsEmpty() && X == 0 && Y == 0)
			{
				Height = BottomLeftNeighborVertices[(RowColumnSize * RowColumnSize) - 1].Z;
			}
			// Bottom Right
			else if (!BottomRightNeighborVertices.IsEmpty() && X == 0 && Y == VERTEX_SIZE)
			{
				Height = BottomRightNeighborVertices[RowColumnSize * VERTEX_SIZE].Z;
			}

			const float Offset = (VERTEX_SIZE * VERTEX_DISTANCE_SCALE) * 0.5f;

			Vertices.Add(FVector((X * VERTEX_DISTANCE_SCALE) - Offset, (Y * VERTEX_DISTANCE_SCALE) - Offset, Height));
			VertexColors.Add(TerrainColor);
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

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);

	MeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	MeshComponent->SetMaterial(0, Material);
}

bool AChunk::GetRandomPointOnTerrain(FTransform& OutTransform)
{
	const int32 Point = FMath::RandRange(0, Vertices.Num() - 1);
	if (!Vertices.IsValidIndex(Point))
	{
		return false;
	}

	OutTransform.SetLocation(Vertices[Point] + GetActorLocation());
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
