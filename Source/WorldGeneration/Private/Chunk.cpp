// Copyright Telephone Studios. All Rights Reserved.


#include "Chunk.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	XSize = 10;
	YSize = 10;
	Scale = 100.0f;
	UVScale = 1.0f;
	Material = nullptr;

}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	CreateVerticies();
	CreateTriangles();

	MeshComponent->CreateMeshSection(0, Verticies, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	MeshComponent->SetMaterial(0, Material);
}

void AChunk::CreateVerticies()
{
	for (uint32 X = 0; X <= XSize; ++X)
	{
		for (uint32 Y = 0; Y <= YSize; ++Y)
		{
			Verticies.Add(FVector(X * Scale, Y * Scale, 0));
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
