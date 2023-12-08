// Copyright Telephone Studios. All Rights Reserved.


#include "Chunk.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	XSize = 16;
	YSize = 16;

	ZScale = 100.0f;
	NoiseScale = 0.1f;

	Scale = 100.0f;
	UVScale = 1.0f;
	Material = nullptr;

	Tags.Add(TEXT("Ground"));
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	Verticies.Reset();
	Triangles.Reset();
	UVs.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateVerticies();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Verticies, Triangles, UVs, Normals, Tangents);

	MeshComponent->CreateMeshSection(0, Verticies, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);
	MeshComponent->SetMaterial(0, Material);
}

void AChunk::CreateVerticies()
{
	for (uint32 X = 0; X <= XSize; ++X)
	{
		for (uint32 Y = 0; Y <= YSize; ++Y)
		{
			const uint32 Seed = FMath::RandRange(0, 1000000);
			const float Z = FMath::PerlinNoise2D(FVector2D(static_cast<float>(X + Seed) * NoiseScale, static_cast<float>(Y + Seed) * NoiseScale)) * ZScale;
			Verticies.Add(FVector(X * Scale, Y * Scale, Z));
			UVs.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void AChunk::CreateTriangles()
{
	uint32 Vertex = 0;
	for (uint32 X = 0; X < XSize; ++X)
	{
		for (uint32 Y = 0; Y < YSize; ++Y)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);

			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			++Vertex;
		}
		++Vertex;
	}
}
