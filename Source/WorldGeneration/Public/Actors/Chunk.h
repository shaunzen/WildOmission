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

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Generate(const TArray<struct FChunkData>& Neighbors);

	UFUNCTION()
	void SetChunkHidden(bool Hidden);

	UFUNCTION()
	void OnLoadFromSaveComplete();

	void Save(struct FChunkData& OutChunkData, bool AlsoDestroy = false);
	void Load(const struct FChunkData& InChunkData);

	static void SetGenerationSeed(const uint32& InSeed);
	static uint32 GetGenerationSeed();

	static float GetContinentalnessAtLocation(const FVector2D& Location, bool UseRawValue = false);
	static float GetErosionAtLocation(const FVector2D& Location, bool UseRawValue = false);
	static float GetPeaksAndValleysAtLocation(const FVector2D& Location, bool UseRawValue = false);

	static void GetTerrainDataAtLocation(const FVector2D& Location, float& OutHeight, FColor& OutTerrainColor);
	void SetChunkLocation(const FIntVector2& InLocation);
	FIntVector2 GetChunkLocation() const;

	void SetVertices(const TArray<FVector>& InVertices);
	TArray<FVector> GetVertices() const;

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
	class UStaticMeshComponent* WaterMeshComponent;

	UPROPERTY(VisibleAnywhere)
	class UChunkSaveComponent* SaveComponent;

	UPROPERTY(SaveGame)
	FIntVector2 GridLocation;

	UPROPERTY(EditAnywhere)
	float UVScale;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MeshData)
	TArray<FVector> Vertices;

	UPROPERTY()
	TArray<int32> Triangles;

	UPROPERTY(SaveGame)
	TArray<FVector2D> UV0;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MeshData, SaveGame)
	TArray<FColor> VertexColors;

	UPROPERTY()
	TArray<FVector> Normals;

	UPROPERTY()
	TArray<struct FProcMeshTangent> Tangents;

	bool ChunkHidden;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_UpdateTerrain(const TArray<FVector>& InVertices, const TArray<FColor>& InVertexColors);

	UFUNCTION()
	void OnRep_MeshData();
	
	void GenerateTerrainShape(const TArray<FChunkData>& Neighbors);
	void GenerateBiome();
	void GenerateDecorations();
	void GenerateSpawnableActors(const TArray<struct FSpawnData>& SpawnDataList);

	bool GetRandomPointOnTerrain(FTransform& OutTransform) const;

	struct FBiomeGenerationData* GetBiomeAtLocation(const FVector2D& Location) const;

	void CreateVertices(const TArray<FChunkData>& Neighbors);
	void CreateTriangles();
	void CreateMesh();

};
