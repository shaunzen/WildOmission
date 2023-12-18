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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void Generate();

	UFUNCTION()
	void OnLoadFromSaveComplete();

	void Save(struct FChunkData& OutChunkData, bool AlsoDestroy = false);
	void Load(const struct FChunkData& InChunkData);

	static void SetGenerationSeed(const uint32& Seed);
	static float GetTerrainHeightAtLocation(const FVector2D& Location, float Scale = 1.0f);
	void SetChunkLocation(const FIntVector2& InLocation);
	FIntVector2 GetChunkLocation() const;

	static int32 GetVertexSize();
	static float GetVertexDistanceScale();
	static float GetMaxHeight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	class UChunkSaveComponent* SaveComponent;

	UPROPERTY(SaveGame)
	FIntVector2 GridLocation;

	// Begin these will be biome specific
	UPROPERTY(EditAnywhere)
	float ZScale;
	UPROPERTY(EditAnywhere)
	float NoiseScale;
	// End these will be biome specific

	UPROPERTY(EditAnywhere)
	float UVScale;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Verticies, SaveGame)
	TArray<FVector> Verticies;

	UPROPERTY()
	TArray<int32> Triangles;

	UPROPERTY(SaveGame)
	TArray<FVector2D> UV0;

	UPROPERTY()
	TArray<FColor> VertexColors;

	UPROPERTY()
	TArray<FVector> Normals;

	UPROPERTY()
	TArray<struct FProcMeshTangent> Tangents;

	UFUNCTION()
	void OnRep_Verticies();

	void GenerateTerrain();
	void GenerateSpawnableActors(const TArray<struct FSpawnData>& SpawnDataList);

	bool GetRandomPointOnTerrain(FTransform& OutTransform);

	FBiomeGenerationData* GetBiomeAtLocation(const FVector2D& Location) const;

	void CreateVerticies();
	void CreateTriangles();
	void CreateMesh();

};
