// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

UCLASS()
class WORLDGENERATION_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	UFUNCTION()
	void Generate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	uint32 Size;

	UPROPERTY(EditAnywhere)
	float ZScale;

	UPROPERTY(EditAnywhere)
	float NoiseScale;

	UPROPERTY(EditAnywhere)
	float Scale;

	UPROPERTY(EditAnywhere)
	float UVScale;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY()
	TArray<FVector> Verticies;

	UPROPERTY()
	TArray<int32> Triangles;

	UPROPERTY()
	TArray<FVector2D> UV0;

	UPROPERTY()
	TArray<FVector> Normals;

	UPROPERTY()
	TArray<struct FProcMeshTangent> Tangents;

	void GenerateTerrain();
	void GenerateTrees();

	bool GetRandomPointOnTerrain(FTransform& OutTransform);

	void CreateVerticies();
	void CreateTriangles();

};
